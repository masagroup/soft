#include "ecore/impl/XmlResourceFactory.hpp"
#include "ecore/impl/XmlResource.hpp"

using namespace ecore;
using namespace ecore::impl;

XmlResourceFactory::XmlResourceFactory()
{
}

XmlResourceFactory::~XmlResourceFactory()
{
}

std::shared_ptr<EResource> XmlResourceFactory::createResource( const URI& uri ) const
{
    auto resource = std::make_shared<XmlResource>();
    resource->setThisPtr(resource);
    return resource;
}
