#include "ecore/impl/XmlHandler.hpp"
#include "ecore/EPackageRegistry.hpp"
#include <codecvt>
#include <iostream>
#include <locale>

using namespace ecore;
using namespace ecore::impl;
using namespace xercesc;

XmlHandler::XmlHandler( XmlResource& resource )
    : resource_( resource )
{
}

XmlHandler::~XmlHandler()
{
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

    std::u16string prefix = namespaces_.getPrefix( uri );
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

std::shared_ptr<EObject> XmlHandler::createObject( const std::shared_ptr<EFactory>& eFactory, const std::shared_ptr<EClassifier>& type ) const
{
    std::shared_ptr<EObject> eObject = ;

}

void XmlHandler::createTopObject( const std::u16string& prefix, const std::u16string& localName )
{
    std::shared_ptr<EFactory> factory = getFactoryForPrefix( prefix );
    if( factory )
    {

    }
    
}



void XmlHandler::handleFeature( const std::u16string& prefix, const std::u16string& localName )
{
}

namespace
{
    static constexpr char16_t* XSI_URI = u"http://www.w3.org/2001/XMLSchema-instance";
    static constexpr char16_t* SCHEMA_LOCATION = u"schemaLocation";
    static constexpr char16_t* NO_NAMESPACE_SCHEMA_LOCATION = u"noNamespaceSchemaLocation";
} // namespace

void XmlHandler::handleSchemaLocation( const xercesc::Attributes& attrs )
{
    auto xsiSchemaLocation = attrs.getValue( XSI_URI, SCHEMA_LOCATION );
    if( xsiSchemaLocation )
        handleXSISchemaLocation( xsiSchemaLocation );

    auto xsiNoNamespaceSchemLocation = attrs.getValue( XSI_URI, NO_NAMESPACE_SCHEMA_LOCATION );
    if( xsiNoNamespaceSchemLocation )
        handleXSINoNamespaceSchemaLocation( xsiNoNamespaceSchemLocation );
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
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        auto uri_utf16 = namespaces_.getUri( prefix );
        auto uri_utf8 = convert.to_bytes( uri_utf16 );
        factory = EPackageRegistry::getInstance()->getFactory( uri_utf8 );
        if( factory )
            prefixesToFactories_.emplace( prefix, factory );
    }
    return factory;
}
