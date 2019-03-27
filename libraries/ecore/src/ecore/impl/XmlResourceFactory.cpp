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

std::shared_ptr<EResource> XmlResourceFactory::createResource( const Uri& uri ) const
{
    return std::make_shared<XmlResource>( uri );
}
