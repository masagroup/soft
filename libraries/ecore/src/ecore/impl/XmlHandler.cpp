#include "ecore/impl/XmlHandler.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EPackageRegistry.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EResource.hpp"
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

namespace
{
    std::vector<std::string_view> split( const std::string& s, const std::string& token )
    {
        std::string_view sv = s;
        std::vector<std::string_view> result;
        std::size_t pos = 0;
        std::size_t start = 0;
        while( ( pos = sv.find( token, start ) ) != std::string::npos )
            result.emplace_back( sv.substr( start, pos ) );
        return result;
    }
} // namespace

XmlHandler::XmlHandler( XmlResource& resource )
    : resource_( resource )
{
}

XmlHandler::~XmlHandler()
{
}

void XmlHandler::setDocumentLocator( const Locator* const locator )
{
    locator_ = locator;
}

void XmlHandler::startDocument()
{
    isRoot_ = true;
    isPushContext_ = true;
    namespaces_.pushContext();
}

void XmlHandler::endDocument()
{
    namespaces_.popContext();
}

void XmlHandler::startElement( const XMLCh* const uri,
                               const XMLCh* const localname,
                               const XMLCh* const qname,
                               const xercesc::Attributes& attrs )
{
    setAttributes( &attrs );
    startElement( utf16_to_utf8( uri ), utf16_to_utf8( localname ), utf16_to_utf8( qname ) );
}

void XmlHandler::startElement( const std::string uri, const std::string& localName, const std::string& qname )
{
    if( isPushContext_ )
        namespaces_.pushContext();
    isPushContext_ = true;

    elements_.push( qname );

    if( isRoot_ )
        handleSchemaLocation();

    processElement( qname, namespaces_.getPrefix( uri ), localName  );
}


void XmlHandler::endElement( const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname )
{
    // pop namespace context and remove corresponding namespace factories
    auto prefixes = namespaces_.popContext();
    for( auto p : prefixes )
        prefixesToFactories_.extract( p.first );
}

void XmlHandler::startPrefixMapping( const XMLCh* const prefix, const XMLCh* const uri )
{
    isNamespaceAware_ = true;
    if( isPushContext_ )
    {
        namespaces_.pushContext();
        isPushContext_ = false;
    }
    auto uri_utf8 = utf16_to_utf8( uri );
    auto prefix_utf8 = utf16_to_utf8( prefix );
    namespaces_.declarePrefix( prefix_utf8, uri_utf8 );
    prefixesToFactories_.extract( prefix_utf8 );
}

void XmlHandler::endPrefixMapping( const XMLCh* const prefix )
{
}

void XmlHandler::characters( const XMLCh* const chars, const XMLSize_t length )
{
}

void XmlHandler::error( const SAXParseException& exc )
{
    error( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlHandler::fatalError( const SAXParseException& exc )
{
    error( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlHandler::warning( const SAXParseException& exc )
{
    warning( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlHandler::processElement( const std::string& name,
                                 const std::string& prefix,
                                 const std::string& localName )
{
    isRoot_ = false;

    if( objects_.empty() )
    {
        auto eObject = createObject( prefix, localName );
        if( eObject )
        {
            handleAttributes( eObject );
            objects_.push( eObject );
            resource_.getContents()->add( eObject );
        }
    }
    else
        handleFeature( prefix, localName );
}

std::shared_ptr<EObject> XmlHandler::createObject( const std::string& prefix, const std::string& name )
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
        handleUnknownPackage( namespaces_.getUri( prefix ) );
        return nullptr;
    }
}

std::shared_ptr<EObject> XmlHandler::createObject( const std::shared_ptr<EFactory>& eFactory, const std::shared_ptr<EClassifier>& eType )
{
    if( eFactory )
    {
        auto eClass = std::dynamic_pointer_cast<EClass>( eType );
        if( eClass && !eClass->isAbstract() )
            return eFactory->create( eClass );
    }
    return nullptr;
}


std::shared_ptr<EObject> XmlHandler::createObjectFromFeatureType( const std::shared_ptr<EObject>& eObject,
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

std::shared_ptr<EObject> XmlHandler::createObjectFromTypeName( const std::shared_ptr<EObject>& eObject,
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
    if( !eFactory && prefix.empty() && namespaces_.getUri( prefix ).empty() )
        handleUnknownPackage( "" );

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

XmlHandler::FeatureKind XmlHandler::getFeatureKind( const std::shared_ptr<EStructuralFeature>& eFeature ) const
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

void XmlHandler::setFeatureValue( const std::shared_ptr<EObject>& eObject,
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
        auto eList = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>( eObject->eGet(eFeature) );
        if( position == -2 )
        {

        }
        else if( value.empty() )
            eList->add( std::shared_ptr<EObject>() );
        else
        {
            auto any = eFactory->createFromString( eDataType, anyCast<std::string>( value ) );
            auto eValue = anyCast<std::shared_ptr<EObject>>( any );
            eList->add( eValue );
        }
        
    }
    case ManyAdd:
    case ManyMove:
    {
        auto eList = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>( eObject->eGet( eFeature ) );
        auto eValue = anyCast<std::shared_ptr<EObject>>( value );
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
                eList->move( position, index );
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

void XmlHandler::setValueFromId( const std::shared_ptr<EObject>& eObject,
                                 const std::shared_ptr<EReference>& eReference,
                                 const std::string& ids )
{
    std::vector<std::string_view> tokens = split( ids, " " );
    std::string qName;
    for( auto token : tokens )
    {
        std::string id;
        std::size_t index = token.find( '#' );
        if( index != std::string_view::npos )
        {
            if( index == 0 )
                id = token.substr( 1 );
            else
            {
                std::shared_ptr<EObject> eProxy = qName.empty() ? createObjectFromFeatureType( eObject, eReference )
                                                                : createObjectFromTypeName( eObject, qName, eReference );
                if( eProxy )
                    eProxy->eSetProxyURI( Uri( id ) );
                objects_.pop();
                qName.clear();
                continue;
            }
        }
        else if( token.find( ':' ) != std::string_view::npos )
        {
            qName = token;
            continue;
        }

        qName.clear();
    }
}

std::string XmlHandler::getLocation() const
{
    return locator_ && locator_->getSystemId() ? utf16_to_utf8( locator_->getSystemId() ) : resource_.getUri().toString();
}

int XmlHandler::getLineNumber() const
{
    return locator_ ? static_cast<int>( locator_->getLineNumber() ) : -1;
}

int XmlHandler::getColumnNumber() const
{
    return locator_ ? static_cast<int>( locator_->getColumnNumber() ) : -1;
}

void XmlHandler::handleFeature( const std::string& prefix, const std::string& name )
{
    using namespace utf16;
    auto xsiType = isNamespaceAware_ ? ( attributes_ ? attributes_->getValue( XSI_URI, TYPE ) : attributes_->getValue( TYPE_ATTRIB ) ) : nullptr;
    handleFeature( prefix, name, xsiType ? utf16_to_utf8( xsiType ) : std::string() );
}

void XmlHandler::handleFeature( const std::string& prefix, const std::string& name, const std::string& type )
{
    std::shared_ptr<EObject> eObject;
    if( !objects_.empty() )
    {
        eObject = objects_.top();
        objects_.pop();
    }

    if( eObject )
    {
        auto eFeature = getFeature( eObject, name );
        if( eFeature )
        {
            if( type.empty() )
                createObjectFromFeatureType( eObject, eFeature );
            else
                createObjectFromTypeName( eObject, type, eFeature );
        }
        else
            handleUnknownFeature( name );
    }
    else
        handleUnknownFeature( name );
}

void XmlHandler::handleUnknownFeature( const std::string& name )
{
    error( std::make_shared<Diagnostic>( "Feature " + name + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

void XmlHandler::handleUnknownPackage( const std::string& name )
{
    error( std::make_shared<Diagnostic>( "Package " + name + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

const Attributes* XmlHandler::setAttributes( const xercesc::Attributes* attrs )
{
    const Attributes* oldAttributes = attributes_;
    attributes_ = attrs;
    return oldAttributes;
}



void XmlHandler::handleAttributes( const std::shared_ptr<EObject>& eObject )
{
    using namespace utf8;
    if( attributes_ )
    {
        for( int i = 0; i < attributes_->getLength(); ++i )
        {
            auto name = utf16_to_utf8( attributes_->getQName( i ) );
            auto value = utf16_to_utf8( attributes_->getValue( i ) );
            if( isNamespaceAware_ )
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

void XmlHandler::setAttributeValue( const std::shared_ptr<EObject>& eObject, const std::string& name, const std::string& value )
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


void XmlHandler::handleSchemaLocation()
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

void XmlHandler::handleXSISchemaLocation( const std::string& schemaLocation )
{
}

void XmlHandler::handleXSINoNamespaceSchemaLocation( const std::string& schemaLocation )
{
}

std::shared_ptr<EFactory> XmlHandler::getFactoryForPrefix( const std::string& prefix )
{
    std::shared_ptr<EFactory> factory;
    auto found = prefixesToFactories_.find( prefix );
    if( found != prefixesToFactories_.end() )
        factory = found->second;
    else
    {
        auto uri = namespaces_.getUri( prefix );
        factory = EPackageRegistry::getInstance()->getFactory( uri );
        if( factory )
            prefixesToFactories_.emplace( prefix, factory );
    }
    return factory;
}

std::shared_ptr<EStructuralFeature> XmlHandler::getFeature( const std::shared_ptr<EObject>& eObject, const std::string& name )
{
    auto eClass = eObject->eClass();
    auto eFeature = eClass->getEStructuralFeature( name );
    return eFeature;
}

void XmlHandler::error( const std::shared_ptr<EDiagnostic>& diagnostic )
{
    resource_.getErrors()->add( diagnostic );
}

void XmlHandler::warning( const std::shared_ptr<EDiagnostic>& diagnostic )
{
    resource_.getWarnings()->add( diagnostic );
}
