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
}

void XmlHandler::endDocument()
{
}

void XmlHandler::startElement( const XMLCh* const uri,
                                            const XMLCh* const localname,
                                            const XMLCh* const qname,
                                            const xercesc::Attributes& attrs )
{
    elements_.push( qname );
  

}

void XmlHandler::startPrefixMapping( const XMLCh* const prefix, const XMLCh* const uri )
{
    prefixesToNamespaces_[prefix] = uri;
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
