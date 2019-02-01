#include "ecore/impl/XmlResource.hpp"
#include "ecore/impl/XmlHandler.hpp"
#include "ecore/impl/XmlInputSource.hpp"
#include "ecore/impl/SaxParserPool.hpp"

using namespace ecore;
using namespace ecore::impl;

XmlResource::XmlResource( const Uri& uri )
    : uri_( uri )
{
}

XmlResource::~XmlResource()
{
}

void XmlResource::doLoad( std::istream& is )
{
    auto& pool = SaxParserPool::getInstance();
    auto parser = pool.getParser();

    XmlHandler handler(*this);
    parser->setContentHandler(&handler);
    
    XmlInputSource source( is );
    parser->parse( source );
}

void XmlResource::doSave( std::ostream& os )
{
}
