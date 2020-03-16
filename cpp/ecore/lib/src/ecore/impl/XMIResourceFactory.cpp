#include "ecore/impl/XMIResource.hpp"
#include "ecore/impl/XMIResourceFactory.hpp"

using namespace ecore;
using namespace ecore::impl;

XMIResourceFactory::XMIResourceFactory()
{
}

XMIResourceFactory::~XMIResourceFactory()
{
}

std::shared_ptr<EResource> XMIResourceFactory::createResource( const URI& uri ) const
{
    auto resource = std::make_shared<XMIResource>();
    resource->setThisPtr( resource );
    return resource;
}
