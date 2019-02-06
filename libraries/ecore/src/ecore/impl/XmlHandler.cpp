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

namespace
{
    static constexpr char16_t* XSI_URI = u"http://www.w3.org/2001/XMLSchema-instance";
    static constexpr char16_t* XSI_NS = u"xsi";
    static constexpr char16_t* XML_NS = u"xmlns";
    static constexpr char16_t* NIL = u"nil";
    static constexpr char16_t* TYPE = u"type";
    static constexpr char16_t* SCHEMA_LOCATION = u"schemaLocation";
    static constexpr char16_t* NO_NAMESPACE_SCHEMA_LOCATION = u"noNamespaceSchemaLocation";
    static std::u16string TYPE_ATTRIB = std::u16string( XSI_NS ) + u":" + TYPE;
    static std::u16string NIL_ATTRIB = std::u16string( XSI_NS ) + u":" + NIL;
    static std::u16string SCHEMA_LOCATION_ATTRIB = std::u16string( XSI_NS ) + u":" + SCHEMA_LOCATION;
    static std::u16string NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB = std::u16string( XSI_NS ) + u":" + NO_NAMESPACE_SCHEMA_LOCATION;
    static std::unordered_set<std::u16string> NOT_FEATURES = {TYPE_ATTRIB, SCHEMA_LOCATION_ATTRIB, NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB};

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

    elements_.push( qname );

    if( isRoot_ )
        handleSchemaLocation( attrs );

    processElement( qname, namespaces_.getPrefix( uri ), localname, attrs );
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
    namespaces_.declarePrefix( prefix, uri );
    prefixesToFactories_.extract( prefix );
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

void XmlHandler::processElement( const std::u16string& name,
                                 const std::u16string& prefix,
                                 const std::u16string& localName,
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
                                  const std::u16string& value,
                                  int position )
{
}

void XmlHandler::setValueFromId( const std::shared_ptr<EObject>& eObject,
                                 const std::shared_ptr<EReference>& eReference,
                                 const std::u16string& ids )
{
}

std::string XmlHandler::getLocation() const
{
    return locator_ && locator_->getSystemId() ? utf16_to_utf8( locator_->getSystemId() ) : resource_.getUri().toString();
}

int XmlHandler::getLineNumber() const
{
    return locator_ ? static_cast<int>(locator_->getLineNumber()) : -1;
}

int XmlHandler::getColumnNumber() const
{
    return locator_ ? static_cast<int> (locator_->getColumnNumber()) : -1;
}

std::shared_ptr<EObject> XmlHandler::createObject( const std::u16string& prefix, const std::u16string& name )
{
    std::shared_ptr<EFactory> eFactory = getFactoryForPrefix( prefix );
    if( eFactory )
    {
        auto ePackage = eFactory->getEPackage();
        auto eType = ePackage->getEClassifier( utf16_to_utf8( name ) );
        return createObject( eFactory, eType );
    }
    else
    {
        handleUnknownPackage( namespaces_.getUri( prefix ) );
        return nullptr;
    }
    
}

void XmlHandler::handleFeature( const std::u16string& prefix, const std::u16string& localName )
{
}

void XmlHandler::handleUnknownFeature( const std::u16string& name, const std::shared_ptr<EObject>& eObject )
{
    resource_.getErrors()->add( std::make_shared<Diagnostic>(
        "Feature " + utf16_to_utf8( name ) + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

void XmlHandler::handleUnknownPackage( const std::u16string& name )
{
    resource_.getErrors()->add( std::make_shared<Diagnostic>(
        "Package " + utf16_to_utf8( name ) + "not found", getLocation(), getLineNumber(), getColumnNumber() ) );
}

void XmlHandler::handleAttributes( const std::shared_ptr<EObject>& eObject, const xercesc::Attributes& attrs )
{
    for( int i = 0; i < attrs.getLength(); ++i )
    {
        std::u16string name = attrs.getQName( i );
        std::u16string value = attrs.getValue( i );
        if( isNamespaceAware_ )
        {
            auto uri = attrs.getURI( i );
            if( uri != XSI_URI )
                setAttributeValue( eObject, name, value );
        }
        else if( !startsWith( name, XML_NS ) && NOT_FEATURES.find( name ) == NOT_FEATURES.end() )
            setAttributeValue( eObject, name, value );
    }
}

void XmlHandler::setAttributeValue( const std::shared_ptr<EObject>& eObject, const std::u16string& name, const std::u16string& value )
{

    std::size_t index = name.find( ':' );
    std::u16string prefix;
    std::u16string localName = name;
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
    auto xsiSchemaLocation = attrs.getValue( XSI_URI, SCHEMA_LOCATION );
    if( xsiSchemaLocation )
        handleXSISchemaLocation( xsiSchemaLocation );

    auto xsiNoNamespaceSchemaLocation = attrs.getValue( XSI_URI, NO_NAMESPACE_SCHEMA_LOCATION );
    if( xsiNoNamespaceSchemaLocation )
        handleXSINoNamespaceSchemaLocation( xsiNoNamespaceSchemaLocation );
}

void XmlHandler::handleXSISchemaLocation( const std::u16string& schemaLocation )
{
}

void XmlHandler::handleXSINoNamespaceSchemaLocation( const std::u16string& schemaLocation )
{
}

std::shared_ptr<EFactory> XmlHandler::getFactoryForPrefix( const std::u16string& prefix )
{
    std::shared_ptr<EFactory> factory;
    auto found = prefixesToFactories_.find( prefix );
    if( found != prefixesToFactories_.end() )
        factory = found->second;
    else
    {
        auto uri_utf16 = namespaces_.getUri( prefix );
        auto uri_utf8 = utf16_to_utf8( uri_utf16 );
        factory = EPackageRegistry::getInstance()->getFactory( uri_utf8 );
        if( factory )
            prefixesToFactories_.emplace( prefix, factory );
    }
    return factory;
}

std::shared_ptr<EStructuralFeature> XmlHandler::getFeature( const std::shared_ptr<EObject>& eObject, const std::u16string& name )
{
    auto eClass = eObject->eClass();
    auto eFeature = eClass->getEStructuralFeature( utf16_to_utf8( name ) );
    return eFeature;
}
