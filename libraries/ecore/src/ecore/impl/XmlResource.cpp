#include "ecore/impl/XmlResource.hpp"
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
}

void XmlResource::doSave( std::ostream& os )
{
}
