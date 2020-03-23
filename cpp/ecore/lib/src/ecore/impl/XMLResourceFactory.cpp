#include "ecore/impl/XMLResourceFactory.hpp"
#include "ecore/impl/XMLResource.hpp"

using namespace ecore;
using namespace ecore::impl;

XMLResourceFactory::XMLResourceFactory()
{
}

XMLResourceFactory::~XMLResourceFactory()
{
}

std::shared_ptr<EResource> XMLResourceFactory::createResource( const URI& uri ) const
{
    auto resource = std::make_shared<XMLResource>( uri );
    resource->setThisPtr(resource);
    return resource;
}
