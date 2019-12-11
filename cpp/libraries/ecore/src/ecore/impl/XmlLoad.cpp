#include "ecore/impl/XmlLoad.hpp"
#include "ecore/Any.hpp"
#include "ecore/AnyCast.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EPackageRegistry.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EResource.hpp"
#include "ecore/EResourceSet.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/impl/Diagnostic.hpp"
#include "ecore/impl/StringUtils.hpp"
#include "ecore/impl/XmlResource.hpp"

#include <iostream>
#include <unordered_set>

using namespace ecore;
using namespace ecore::impl;
using namespace xercesc;

namespace utf8
{

    static constexpr char* XSI_URI = "http://www.w3.org/2001/XMLSchema-instance";
    static constexpr char* XSI_NS = "xsi";
    static constexpr char* XML_NS = "xmlns";
    static constexpr char* NIL = "nil";
    static constexpr char* TYPE = "type";
    static constexpr char* HREF = "href";
    static constexpr char* SCHEMA_LOCATION = "schemaLocation";
    static constexpr char* NO_NAMESPACE_SCHEMA_LOCATION = "noNamespaceSchemaLocation";
    static constexpr char* TYPE_ATTRIB = "xsi:type";
    static constexpr char* NIL_ATTRIB = "xsi:nil";
    static constexpr char* SCHEMA_LOCATION_ATTRIB = "xsi:schemaLocation";
    static constexpr char* NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB = "xsi:noNamespaceSchemaLocation";
    static std::unordered_set<std::string> NOT_FEATURES = {TYPE_ATTRIB, SCHEMA_LOCATION_ATTRIB, NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB};

} // namespace utf8
namespace utf16
{
    static constexpr char16_t* XSI_URI = u"http://www.w3.org/2001/XMLSchema-instance";
    static constexpr char16_t* SCHEMA_LOCATION = u"schemaLocation";
    static constexpr char16_t* TYPE = u"type";
    static constexpr char16_t* TYPE_ATTRIB = u"xsi:type";
    static constexpr char16_t* NO_NAMESPACE_SCHEMA_LOCATION = u"noNamespaceSchemaLocation";
} // namespace utf16

XmlLoad::XmlLoad( XmlResource& resource )
    : resource_( resource )
    , packageRegistry_( resource_.getResourceSet() ? resource_.getResourceSet()->getPackageRegistry() : EPackageRegistry::getInstance() )
{
}

XmlLoad::~XmlLoad()
{
}

void XmlLoad::setDocumentLocator( const Locator* const locator )
{
    locator_ = locator;
}

void XmlLoad::startDocument()
{
    isRoot_ = true;
    isPushContext_ = true;
    namespaces_.pushContext();
}

void XmlLoad::endDocument()
{
    auto _ = namespaces_.popContext();
    handleReferences();
}

void XmlLoad::startElement( const XMLCh* const uri,
                               const XMLCh* const localname,
                               const XMLCh* const qname,
                               const xercesc::Attributes& attrs )
{
    setAttributes( &attrs );
    startElement( utf16_to_utf8( uri ), utf16_to_utf8( localname ), utf16_to_utf8( qname ) );
}

void XmlLoad::startElement( const std::string uri, const std::string& localName, const std::string& qname )
{
    if( isPushContext_ )
        namespaces_.pushContext();
    isPushContext_ = true;

    elements_.push( qname );

    if( isRoot_ )
        handleSchemaLocation();

    processElement( qname, namespaces_.getPrefix( uri ), localName );
}

void XmlLoad::endElement( const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname )
{
    objects_.pop();

    // pop namespace context and remove corresponding namespace factories
    auto prefixes = namespaces_.popContext();
    for( auto p : prefixes )
        auto _ =  prefixesToFactories_.extract( p.first );
}

void XmlLoad::startPrefixMapping( const XMLCh* const prefix, const XMLCh* const uri )
{
    isNamespaceAware_ = true;
    if( isPushContext_ )
    {
        namespaces_.pushContext();
        isPushContext_ = false;
    }
    auto uri_utf8 = utf16_to_utf8( uri );
    auto prefix_utf8 = utf16_to_utf8( prefix );
    auto _ = prefixesToFactories_.extract(prefix_utf8);
    namespaces_.declarePrefix( prefix_utf8, uri_utf8 );
    
}

void XmlLoad::endPrefixMapping( const XMLCh* const prefix )
{
}

void XmlLoad::characters( const XMLCh* const chars, const XMLSize_t length )
{
}

void XmlLoad::error( const SAXParseException& exc )
{
    error( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlLoad::fatalError( const SAXParseException& exc )
{
    error( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlLoad::warning( const SAXParseException& exc )
{
    warning( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlLoad::processElement( const std::string& name, const std::string& prefix, const std::string& localName )
{
    isRoot_ = false;

    if( objects_.empty() )
    {
        auto eObject = createObject( prefix, localName );
        if( eObject )
        {
            objects_.push( eObject );
            resource_.getContents()->add( eObject );
        }
    }
    else
        handleFeature( prefix, localName );
}

std::shared_ptr<EObject> XmlLoad::createObject( const std::string& prefix, const std::string& name )
{
    std::shared_ptr<EFactory> eFactory = getFactoryForPrefix( prefix );
    if( eFactory )
    {
        auto ePackage = eFactory->getEPackage();
        auto eType = ePackage->getEClassifier( name );
        return createObject( eFactory, eType );
    }
    else
    {
        handleUnknownPackage( namespaces_.getURI( prefix ) );
        return nullptr;
    }
}

std::shared_ptr<EObject> XmlLoad::createObject( const std::shared_ptr<EFactory>& eFactory, const std::shared_ptr<EClassifier>& eType )
{
    if( eFactory )
    {
        auto eClass = std::dynamic_pointer_cast<EClass>( eType );
        if( eClass && !eClass->isAbstract() )
        {
            auto eObject = eFactory->create( eClass );
            if( eObject )
                handleAttributes( eObject );
            return eObject;
        }
    }
    return nullptr;
}

std::shared_ptr<EObject> XmlLoad::createObjectFromFeatureType( const std::shared_ptr<EObject>& eObject,
                                                                  const std::shared_ptr<EStructuralFeature>& eFeature )
{
    std::shared_ptr<EObject> eResult;
    if( eFeature && eFeature->getEType() )
    {
        auto eType = eFeature->getEType();
        auto eFactory = eType->getEPackage()->getEFactoryInstance();
        eResult = createObject( eFactory, eType );
    }
    if( eResult )
    {
        setFeatureValue( eObject, eFeature, eResult );
        objects_.push( eResult );
    }
    return eResult;
}

std::shared_ptr<EObject> XmlLoad::createObjectFromTypeName( const std::shared_ptr<EObject>& eObject,
                                                               const std::string& typeQName,
                                                               const std::shared_ptr<EStructuralFeature>& eFeature )
{
    std::string typeName;
    std::string prefix = "";
    std::size_t index = typeQName.find( ':' );
    if( index > 0 )
    {
        prefix = typeQName.substr( 0, index );
        typeName = typeQName.substr( index + 1 );
    }
    else
        typeName = typeQName;

    auto eFactory = getFactoryForPrefix( prefix );
    if( !eFactory && prefix.empty() && namespaces_.getURI( prefix ).empty() )
        handleUnknownPackage( prefix );

    auto ePackage = eFactory->getEPackage();
    auto eType = ePackage->getEClassifier( typeName );
    auto eResult = createObject( eFactory, eType );
    if( eResult )
    {
        setFeatureValue( eObject, eFeature, eResult );
        objects_.push( eResult );
    }
    return eResult;
}

XmlLoad::FeatureKind XmlLoad::getFeatureKind( const std::shared_ptr<EStructuralFeature>& eFeature ) const
{
    auto eClassifier = eFeature->getEType();
    auto eDataType = std::dynamic_pointer_cast<EDataType>( eClassifier );
    if( eDataType )
    {
        if( eFeature->isMany() )
            return Many;
        else
            return Single;
    }
    else if( eFeature->isMany() )
    {
        auto eReference = std::dynamic_pointer_cast<EReference>( eFeature );
        auto eOpposite = eReference->getEOpposite();
        if( !eOpposite || eOpposite->isTransient() || !eOpposite->isMany() )
            return ManyAdd;
        else
            return ManyMove;
    }
    else
        return Other;
}

void XmlLoad::setFeatureValue( const std::shared_ptr<EObject>& eObject,
                                  const std::shared_ptr<EStructuralFeature>& eFeature,
                                  const Any& value,
                                  int position )
{
    int kind = getFeatureKind( eFeature );
    switch( kind )
    {
    case Single:
    {
        auto eClassifier = eFeature->getEType();
        auto eDataType = std::dynamic_pointer_cast<EDataType>( eClassifier );
        auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
        if( value.empty() )
            eObject->eSet( eFeature, Any() );
        else
            eObject->eSet( eFeature, eFactory->createFromString( eDataType, anyCast<std::string>( value ) ) );
        break;
    }
    case Many:
    {
        auto eClassifier = eFeature->getEType();
        auto eDataType = std::dynamic_pointer_cast<EDataType>( eClassifier );
        auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
        auto eList = anyListCast<std::shared_ptr<EObject>>( eObject->eGet( eFeature ) );
        if( position == -2 )
        {
        }
        else if( value.empty() )
            eList->add( std::shared_ptr<EObject>() );
        else
        {
            auto any = eFactory->createFromString( eDataType, anyCast<std::string>( value ) );
            auto eValue = anyObjectCast<std::shared_ptr<EObject>>( any );
            eList->add( eValue );
        }
    }
    case ManyAdd:
    case ManyMove:
    {
        auto eList = anyListCast<std::shared_ptr<EObject>>( eObject->eGet( eFeature ) );
        auto eValue = anyObjectCast<std::shared_ptr<EObject>>( value );
        if( position == -1 )
            eList->add( eValue );
        else if( position == -2 )
            eList->clear();
        else if( eObject == eValue )
        {
            std::size_t index = eList->indexOf( eValue );
            if( index == -1 )
                eList->add( position, eValue );
            else
                auto _ = eList->move( position, index );
        }
        else if( kind == ManyAdd )
            eList->add( eValue );
        else
            eList->move( position, eValue );
        break;
    }
    default:
    {
        eObject->eSet( eFeature, value );
        break;
    }
    }
}

void XmlLoad::setAttributeValue( const std::shared_ptr<EObject>& eObject, const std::string& name, const std::string& value )
{

    std::size_t index = name.find( ':' );
    std::string prefix;
    std::string localName = name;
    if( index != -1 )
    {
        prefix = name.substr( 0, index );
        localName = name.substr( index + 1 );
    }
    auto eFeature = getFeature( eObject, localName );
    if( eFeature )
    {
        FeatureKind kind = getFeatureKind( eFeature );
        if( kind == Single || kind == Many )
            setFeatureValue( eObject, eFeature, value, -2 );
        else
            setValueFromId( eObject, std::dynamic_pointer_cast<EReference>( eFeature ), value );
    }
    else
        handleUnknownFeature( localName );
}

struct XmlLoad::Reference
{
    std::shared_ptr<EObject> object_;
    std::shared_ptr<EStructuralFeature> feature_;
    std::string id_;
    int pos_;
    int line_;
    int column_;
};

void XmlLoad::setValueFromId( const std::shared_ptr<EObject>& eObject,
                                 const std::shared_ptr<EReference>& eReference,
                                 const std::string& ids )
{
    bool mustAdd = isResolveDeferred_;
    bool mustAddOrNotOppositeIsMany = false;
    bool isFirstID = true;
    int position = 0;
    int line = getLineNumber();
    int column = getColumnNumber();
    std::vector<Reference> references;
    std::string qName;
    auto tokens = split(ids, " ");
    for( auto token : tokens )
    {
        std::string id(token);
        std::size_t index = id.find( '#' );
        if( index != std::string_view::npos )
        {
            if( index == 0 )
                id = token.substr( 1 );
            else
            {
                auto oldAttributes = setAttributes( nullptr );
                std::shared_ptr<EObject> eProxy = qName.empty() ? createObjectFromFeatureType( eObject, eReference )
                                                                : createObjectFromTypeName( eObject, qName, eReference );
                setAttributes( oldAttributes );
                if( eProxy )
                    handleProxy( eProxy, id );
                objects_.pop();
                qName.clear();
                ++position;
                continue;
            }
        }
        else if( id.find( ':' ) != std::string_view::npos )
        {
            qName = id;
            continue;
        }

        if( !isResolveDeferred_ )
        {
            if( isFirstID )
            {
                auto eOpposite = eReference->getEOpposite();
                if( eOpposite )
                {
                    mustAdd = eOpposite->isTransient() || eReference->isMany();
                    mustAddOrNotOppositeIsMany = mustAdd || !eOpposite->isMany();
                }
                else
                {
                    mustAdd = true;
                    mustAddOrNotOppositeIsMany = true;
                }
                isFirstID = false;
            }

            if( mustAddOrNotOppositeIsMany )
            {
                auto resolved = resource_.getEObject( id );
                if( resolved )
                {
                    setFeatureValue( eObject, eReference, resolved );
                    qName.clear();
                    ++position;
                    continue;
                }
            }
        }

        if( mustAdd )
            references.push_back( {eObject, eReference, id, position, line, column} );

        qName.clear();
        ++position;
    }

    if( position == 0 )
        setFeatureValue( eObject, eReference, Any(), -2 );
    else
        references_ = references;
}

std::string XmlLoad::getLocation() const
{
    return locator_ && locator_->getSystemId() ? utf16_to_utf8( locator_->getSystemId() ) : resource_.getURI().toString();
}

int XmlLoad::getLineNumber() const
{
    return locator_ ? static_cast<int>( locator_->getLineNumber() ) : -1;
}

int XmlLoad::getColumnNumber() const
{
    return locator_ ? static_cast<int>( locator_->getColumnNumber() ) : -1;
}

void XmlLoad::handleFeature( const std::string& prefix, const std::string& name )
{
    std::shared_ptr<EObject> eObject;
    if( !objects_.empty() )
        eObject = objects_.top();

    if( eObject )
    {
        auto eFeature = getFeature( eObject, name );
        if( eFeature )
        {
            using namespace utf16;
            auto xsiType = isNamespaceAware_
                               ? ( attributes_ ? attributes_->getValue( XSI_URI, TYPE ) : attributes_->getValue( TYPE_ATTRIB ) )
                               : nullptr;
            if( xsiType )
                auto _ = createObjectFromTypeName( eObject, utf16_to_utf8( xsiType ), eFeature );

            else
                auto _ = createObjectFromFeatureType( eObject, eFeature );
        }
        else
            handleUnknownFeature( name );
    }
    else
        handleUnknownFeature( name );
}

void XmlLoad::handleUnknownFeature( const std::string& name )
{
    error( std::make_shared<Diagnostic>( "Feature " + name + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

void XmlLoad::handleUnknownPackage( const std::string& name )
{
    error( std::make_shared<Diagnostic>( "Package " + name + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

void XmlLoad::handleReferences()
{
    for( auto eProxy : sameDocumentProxies_ )
    {
        for( auto eReference : *eProxy->eClass()->getEAllReferences() )
        {
            auto eOpposite = eReference->getEOpposite();
            if( eOpposite && eOpposite->isChangeable() && eProxy->eIsSet( eReference ) )
            {
                auto resolvedObject = resource_.getEObject( eProxy->eProxyURI().getFragment() );
                if( resolvedObject )
                {
                    std::shared_ptr<EObject> proxyHolder;
                    if( eReference->isMany() )
                    {
                        auto value = eProxy->eGet( eReference );
                        auto list = anyListCast<std::shared_ptr<EObject>>( value );
                        proxyHolder = list->get( 0 );
                    }
                    else
                    {
                        auto value = eProxy->eGet( eReference );
                        proxyHolder = anyObjectCast<std::shared_ptr<EObject>>( value );
                    }

                    if( eOpposite->isMany() )
                    {
                        auto value = proxyHolder->eGet( eOpposite );
                        auto holderContents = anyListCast<std::shared_ptr<EObject>>( value );
                        auto resolvedIndex = holderContents->indexOf( resolvedObject );
                        if( resolvedIndex != -1 )
                        {
                            auto proxyIndex = holderContents->indexOf( eProxy );
                            auto _1 = holderContents->move( proxyIndex, resolvedIndex );
                            auto _2 = holderContents->remove( proxyIndex > resolvedIndex ? proxyIndex - 1 : proxyIndex + 1 );
                            break;
                        }
                    }

                    auto replace = false;
                    if( eReference->isMany() )
                    {
                        auto value = resolvedObject->eGet( eReference );
                        auto list = anyListCast<std::shared_ptr<EObject>>( value );
                        replace = !list->contains( proxyHolder );
                    }
                    else
                    {
                        auto value = resolvedObject->eGet( eReference );
                        auto object = anyObjectCast<std::shared_ptr<EObject>>( value );
                        replace = object != proxyHolder;
                    }

                    if( replace )
                    {
                        if( eOpposite->isMany() )
                        {
                            auto value = proxyHolder->eGet( eOpposite );
                            auto list = anyListCast<std::shared_ptr<EObject>>( value );
                            auto ndx = list->indexOf( eProxy );
                            list->set( ndx, resolvedObject );
                        }
                        else
                            proxyHolder->eSet( eOpposite, resolvedObject );
                    }

                    break;
                }
            }
        }
    }

    for( auto& reference : references_ )
    {
        auto eObject = resource_.getEObject( reference.id_ );
        if( eObject )
            setFeatureValue( reference.object_, reference.feature_, eObject, reference.pos_ );
        else
            error( std::make_shared<Diagnostic>(
                "Unresolved reference '" + reference.id_ + "'", getLocation(), reference.line_, reference.column_ ) );
    }
}

const Attributes* XmlLoad::setAttributes( const xercesc::Attributes* attrs )
{
    const Attributes* oldAttributes = attributes_;
    attributes_ = attrs;
    return oldAttributes;
}

void XmlLoad::handleAttributes( const std::shared_ptr<EObject>& eObject )
{
    using namespace utf8;
    if( attributes_ )
    {
        for( int i = 0; i < attributes_->getLength(); ++i )
        {
            auto name = utf16_to_utf8( attributes_->getQName( i ) );
            auto value = utf16_to_utf8( attributes_->getValue( i ) );
            if( name == HREF )
                handleProxy( eObject, value );
            else if( isNamespaceAware_ )
            {
                auto uri = utf16_to_utf8( attributes_->getURI( i ) );
                if( uri != XSI_URI )
                    setAttributeValue( eObject, name, value );
            }
            else if( !startsWith( name, XML_NS ) && NOT_FEATURES.find( name ) == NOT_FEATURES.end() )
                setAttributeValue( eObject, name, value );
        }
    }
}

void XmlLoad::handleProxy( const std::shared_ptr<EObject>& eProxy, const std::string& id )
{
    eProxy->eSetProxyURI( URI( id ) );

    auto uri = URI( id );
    if( uri.trimFragment() == resource_.getURI() )
        sameDocumentProxies_.push_back( eProxy );
}

void XmlLoad::handleSchemaLocation()
{
    if( attributes_ )
    {
        using namespace utf16;
        auto xsiSchemaLocation = attributes_->getValue( XSI_URI, SCHEMA_LOCATION );
        if( xsiSchemaLocation )
            handleXSISchemaLocation( utf16_to_utf8( xsiSchemaLocation ) );

        auto xsiNoNamespaceSchemaLocation = attributes_->getValue( XSI_URI, NO_NAMESPACE_SCHEMA_LOCATION );
        if( xsiNoNamespaceSchemaLocation )
            handleXSINoNamespaceSchemaLocation( utf16_to_utf8( xsiNoNamespaceSchemaLocation ) );
    }
}

void XmlLoad::handleXSISchemaLocation( const std::string& schemaLocation )
{
}

void XmlLoad::handleXSINoNamespaceSchemaLocation( const std::string& schemaLocation )
{
}

std::shared_ptr<EFactory> XmlLoad::getFactoryForPrefix( const std::string& prefix )
{
    std::shared_ptr<EFactory> factory;
    auto found = prefixesToFactories_.find( prefix );
    if( found != prefixesToFactories_.end() )
        factory = found->second;
    else
    {
        auto uri = namespaces_.getURI( prefix );
        factory = packageRegistry_->getFactory( uri );
        if( factory )
            prefixesToFactories_.emplace( prefix, factory );
    }
    return factory;
}

std::shared_ptr<EStructuralFeature> XmlLoad::getFeature( const std::shared_ptr<EObject>& eObject, const std::string& name )
{
    auto eClass = eObject->eClass();
    auto eFeature = eClass->getEStructuralFeature( name );
    return eFeature;
}

void XmlLoad::error( const std::shared_ptr<EDiagnostic>& diagnostic )
{
    resource_.getErrors()->add( diagnostic );
}

void XmlLoad::warning( const std::shared_ptr<EDiagnostic>& diagnostic )
{
    resource_.getWarnings()->add( diagnostic );
}
