#include "ecore/impl/XMIResource.hpp"
#include "ecore/impl/SaxParserPool.hpp"
#include "ecore/impl/XMLInputSource.hpp"
#include "ecore/impl/XMILoad.hpp"

using namespace ecore;
using namespace ecore::impl;

XMIResource::XMIResource()
    : XMLResource()
{
}

XMIResource::XMIResource( const URI& uri )
    : XMLResource( uri )
{
}

XMIResource::~XMIResource()
{
}

std::unique_ptr<AbstractXMLLoad> ecore::impl::XMIResource::createXMLLoad()
{
    return std::move( std::make_unique<XMILoad>( *this ) );
}

