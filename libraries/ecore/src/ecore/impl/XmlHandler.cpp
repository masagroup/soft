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
    static std::string XSI_URI = "http://www.w3.org/2001/XMLSchema-instance";
    static std::string XSI_NS = "xsi";
    static std::string XML_NS = "xmlns";
    static std::string NIL = "nil";
    static std::string TYPE = "type";
    static std::string SCHEMA_LOCATION = "schemaLocation";
    static std::string NO_NAMESPACE_SCHEMA_LOCATION = "noNamespaceSchemaLocation";
    static std::string TYPE_ATTRIB = XSI_NS + ":" + TYPE;
    static std::string NIL_ATTRIB = XSI_NS + ":" + NIL;
    static std::string SCHEMA_LOCATION_ATTRIB = XSI_NS + ":" + SCHEMA_LOCATION;
    static std::string NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB = XSI_NS + ":" + NO_NAMESPACE_SCHEMA_LOCATION;
    static std::unordered_set<std::string> NOT_FEATURES = {TYPE_ATTRIB, SCHEMA_LOCATION_ATTRIB, NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB};

} // namespace utf8
namespace utf16
{
    static constexpr char16_t* XSI_URI = u"http : // www.w3.org/2001/XMLSchema-instance";
    static constexpr char16_t* SCHEMA_LOCATION = u"schemaLocation";
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
    if( isPushContext_ )
        namespaces_.pushContext();
    isPushContext_ = true;

    auto qname_utf8 = utf16_to_utf8( qname );
    auto localname_utf8 = utf16_to_utf8( localname );
    auto uri_utf8 = utf16_to_utf8( uri );
    elements_.push( qname_utf8 );

    if( isRoot_ )
        handleSchemaLocation( attrs );

    processElement( qname_utf8, namespaces_.getPrefix( uri_utf8 ), localname_utf8, attrs );
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
    resource_.getErrors()->add( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlHandler::fatalError( const SAXParseException& exc )
{
    resource_.getErrors()->add( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlHandler::warning( const SAXParseException& exc )
{
    resource_.getWarnings()->add( std::make_shared<Diagnostic>(
        utf16_to_utf8( exc.getMessage() ), "", static_cast<int>( exc.getLineNumber() ), static_cast<int>( exc.getColumnNumber() ) ) );
}

void XmlHandler::processElement( const std::string& name,
                                 const std::string& prefix,
                                 const std::string& localName,
                                 const xercesc::Attributes& attrs )
{
    if( objects_.empty() )
    {
        auto eObject = createObject( prefix, localName );
        if( eObject )
        {
            handleAttributes( eObject, attrs );
            objects_.push( eObject );
        }
    }
    else
        handleFeature( prefix, localName );
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
                                  const std::string& value,
                                  int position )
{
    int kind = getFeatureKind( eFeature );
    switch( kind )
    {
    case Single:
    case Many:
    {
        std::shared_ptr<EClassifier> eClassifier = eFeature->getEType();
        std::shared_ptr<EDataType> eDataType = std::dynamic_pointer_cast<EDataType>( eClassifier );
        std::shared_ptr<EFactory> eFactory = eDataType->getEPackage()->getEFactoryInstance();

        if( kind == Many )
        {
            std::shared_ptr<EList<std::shared_ptr<EObject>>> list = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>( eObject->eGet( eFeature ) );
            if( position == -2 )
            {
            }
            else if( value.empty() )
            {
                list->add( nullptr );
            }
            else
            {
            }
        }
        else if( value.empty() )
            eObject->eSet( eFeature, Any() );
        else
            eObject->eSet( eFeature, eFactory->createFromString( eDataType, value ) );
        break;
    }
    case ManyAdd:
    case ManyMove:
    {
        std::shared_ptr<EList<std::string>> list = anyCast<std::shared_ptr<EList<std::string>>>( eObject->eGet( eFeature ) );
        if( position == -1 )
            list->add( value );
        else if( position == -2 )
            list->clear();
        else
            list->add( position, value );
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

void XmlHandler::handleFeature( const std::string& prefix, const std::string& localName )
{
}

void XmlHandler::handleUnknownFeature( const std::string& name, const std::shared_ptr<EObject>& eObject )
{
    resource_.getErrors()->add(
        std::make_shared<Diagnostic>( "Feature " + name + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

void XmlHandler::handleUnknownPackage( const std::string& name )
{
    resource_.getErrors()->add(
        std::make_shared<Diagnostic>( "Package " + name + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

void XmlHandler::handleAttributes( const std::shared_ptr<EObject>& eObject, const xercesc::Attributes& attrs )
{
    using namespace utf8;

    for( int i = 0; i < attrs.getLength(); ++i )
    {
        auto name = utf16_to_utf8( attrs.getQName( i ) );
        auto value = utf16_to_utf8( attrs.getValue( i ) );
        if( isNamespaceAware_ )
        {
            auto uri = utf16_to_utf8( attrs.getURI( i ) );
            if( uri != XSI_URI )
                setAttributeValue( eObject, name, value );
        }
        else if( !startsWith( name, XML_NS ) && NOT_FEATURES.find( name ) == NOT_FEATURES.end() )
            setAttributeValue( eObject, name, value );
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
        handleUnknownFeature( localName, eObject );
}

void XmlHandler::handleSchemaLocation( const xercesc::Attributes& attrs )
{
    using namespace utf16;

    auto xsiSchemaLocation = attrs.getValue( XSI_URI, SCHEMA_LOCATION );
    if( xsiSchemaLocation )
        handleXSISchemaLocation( utf16_to_utf8( xsiSchemaLocation ) );

    auto xsiNoNamespaceSchemaLocation = attrs.getValue( XSI_URI, NO_NAMESPACE_SCHEMA_LOCATION );
    if( xsiNoNamespaceSchemaLocation )
        handleXSINoNamespaceSchemaLocation( utf16_to_utf8( xsiNoNamespaceSchemaLocation ) );
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
