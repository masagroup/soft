#include "ecore/impl/XmlHandler.hpp"
#include <iostream>

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
