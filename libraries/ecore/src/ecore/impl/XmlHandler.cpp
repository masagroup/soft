#include "ecore/impl/XmlHandler.hpp"
#include <iostream>
using namespace ecore;
using namespace ecore::impl;

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
    std::cout << uri << " " << localname << " " << qname << std::endl;
}

void XmlHandler::startPrefixMapping( const XMLCh* const prefix, const XMLCh* const uri )
{
}

void XmlHandler::characters( const XMLCh* const chars, const XMLSize_t length )
{
}

void XmlHandler::error( const xercesc::SAXParseException& exc )
{
}

void XmlHandler::fatalError( const xercesc::SAXParseException& exc )
{
}

void XmlHandler::warning( const xercesc::SAXParseException& exc )
{
}
