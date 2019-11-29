#include "ecore/impl/XmlResource.hpp"
#include "ecore/impl/SaxParserPool.hpp"
#include "ecore/impl/XmlLoad.hpp"
#include "ecore/impl/XmlInputSource.hpp"

using namespace ecore;
using namespace ecore::impl;

XmlResource::XmlResource()
    : AbstractResource()
{
}

XmlResource::XmlResource( const URI& uri )
    : AbstractResource( uri )
{
}

XmlResource::~XmlResource()
{
}

void XmlResource::doLoad( std::istream& is )
{
    auto& pool = SaxParserPool::getInstance();
    auto parser = pool.getParser();

    XmlLoad handler( *this );
    parser->setContentHandler( &handler );

    XmlInputSource source( is );
    parser->parse( source );
}

void XmlResource::doSave( std::ostream& os )
{
}
