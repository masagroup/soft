#include "ecore/impl/XmlHandler.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EPackageRegistry.hpp"

#include <codecvt>
#include <iostream>
#include <locale>

using namespace ecore;
using namespace ecore::impl;
using namespace xercesc;

namespace
{
    static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> CONVERT_UTF8_UTF16;
    static constexpr char16_t* XSI_URI = u"http://www.w3.org/2001/XMLSchema-instance";
    static constexpr char16_t* SCHEMA_LOCATION = u"schemaLocation";
    static constexpr char16_t* NO_NAMESPACE_SCHEMA_LOCATION = u"noNamespaceSchemaLocation";
} // namespace

XmlHandler::XmlHandler( XmlResource& resource )
    : resource_( resource )
{
}

XmlHandler::~XmlHandler()
{
}

struct XmlHandler::Attribute
{
    std::u16string uri_;
    std::u16string qname_;
    std::u16string localName_;
    std::u16string type_;
    std::u16string value_;
};

void XmlHandler::setAttributes( const xercesc::Attributes& attrs )
{
    attributes_.clear();
    for( int i = 0; i < attrs.getLength(); ++i )
        attributes_.push_back( {attrs.getURI( i ), attrs.getQName( i ), attrs.getLocalName( i ), attrs.getType( i ), attrs.getValue( i )} );
}

XmlHandler::Attribute* XmlHandler::getAttribute( const std::u16string& uri, const std::u16string& localPart )
{
    for( auto& a : attributes_ )
    {
        if( a.uri_ == uri && a.localName_ == localPart )
            return &a;
    }
    return nullptr;
}

XmlHandler::Attribute* XmlHandler::getAttribute( const std::u16string& qname )
{
    for( auto& a : attributes_ )
    {
        if( a.qname_ == qname )
            return &a;
    }
    return nullptr;
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
    setAttributes( attrs );
    startElement( uri, localname, qname );
}

void XmlHandler::startElement( const std::u16string& uri, const std::u16string& localname, const std::u16string& qname )
{
    if( isPushContext_ )
        namespaces_.pushContext();
    isPushContext_ = true;

    elements_.push( qname );

    if( isRoot_ )
        handleSchemaLocation();

    processElement( qname, namespaces_.getPrefix( uri ), localname );
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
}

void XmlHandler::fatalError( const SAXParseException& exc )
{
}

void XmlHandler::warning( const SAXParseException& exc )
{
}

void XmlHandler::processElement( const std::u16string& name, const std::u16string& prefix, const std::u16string& localName )
{
    isRoot_ = false;
    if( objects_.empty() )
        createTopObject( prefix, localName );
    else
        handleFeature( prefix, localName );
}

std::shared_ptr<EObject> XmlHandler::createObject( const std::shared_ptr<EFactory>& eFactory, const std::shared_ptr<EClassifier>& eType )
{
    std::shared_ptr<EObject> eObject;
    if( eFactory )
    {
        auto eClass = std::dynamic_pointer_cast<EClass>( eType );
        if( eClass && !eClass->isAbstract() )
            eObject = eFactory->create( eClass );
    }
    handleObjectAttributes( eObject );
    return eObject;
}

void XmlHandler::createTopObject( const std::u16string& prefix, const std::u16string& name )
{
    std::shared_ptr<EFactory> eFactory = getFactoryForPrefix( prefix );
    if( eFactory )
    {
        auto ePackage = eFactory->getEPackage();
        auto eType = ePackage->getEClassifier( CONVERT_UTF8_UTF16.to_bytes( name ) );
        std::shared_ptr<EObject> eObject = createObject( eFactory, eType );
        if( eObject )
            objects_.push( eObject );
    }
}

void XmlHandler::handleFeature( const std::u16string& prefix, const std::u16string& localName )
{
}

void XmlHandler::handleObjectAttributes( const std::shared_ptr<EObject>& eObject )
{
    if( eObject )
    {

    }
}

void XmlHandler::handleSchemaLocation()
{
    auto xsiSchemaLocation = getAttribute( XSI_URI, SCHEMA_LOCATION );
    if( xsiSchemaLocation )
        handleXSISchemaLocation( xsiSchemaLocation->value_ );

    auto xsiNoNamespaceSchemLocation = getAttribute( XSI_URI, NO_NAMESPACE_SCHEMA_LOCATION );
    if( xsiNoNamespaceSchemLocation )
        handleXSINoNamespaceSchemaLocation( xsiNoNamespaceSchemLocation->value_ );
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
        auto uri_utf8 = CONVERT_UTF8_UTF16.to_bytes( uri_utf16 );
        factory = EPackageRegistry::getInstance()->getFactory( uri_utf8 );
        if( factory )
            prefixesToFactories_.emplace( prefix, factory );
    }
    return factory;
}
