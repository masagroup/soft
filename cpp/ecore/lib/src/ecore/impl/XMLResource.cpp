#include "ecore/impl/XMLResource.hpp"
#include "ecore/impl/SaxParserPool.hpp"
#include "ecore/impl/XMLLoad.hpp"
#include "ecore/impl/XMLInputSource.hpp"
#include "ecore/impl/XMLSave.hpp"

using namespace ecore;
using namespace ecore::impl;

XMLResource::XMLResource()
    : AbstractResource()
{
}

XMLResource::XMLResource( const URI& uri )
    : AbstractResource( uri )
{
}

XMLResource::~XMLResource()
{
}

void XMLResource::doLoad( std::istream& is )
{
    auto& pool = SaxParserPool::getInstance();
    auto parser = pool.getParser();
    auto& reader = parser->getReader();

    auto xmlLoad = createXMLLoad();
    reader.setContentHandler( xmlLoad.get() );
    
    XMLInputSource source( is );
    reader.parse( source );
}

void XMLResource::doSave( std::ostream& os )
{
    auto xmlSave = createXMLSave();
    xmlSave->save( os );
}

std::unique_ptr<XMLLoad> XMLResource::createXMLLoad()
{
    return std::move( std::make_unique<XMLLoad>(*this) );
}

std::unique_ptr<XMLSave> ecore::impl::XMLResource::createXMLSave()
{
    return std::move( std::make_unique<XMLSave>( *this ) );
}

