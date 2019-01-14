#include "ecore/impl/ResourceSet.hpp"

using namespace ecore;
using namespace ecore::impl;

ResourceSet::ResourceSet()
    : resources_( [&]() { return initResources();  } )
{
}

ResourceSet::~ResourceSet()
{
}

std::shared_ptr<EList<std::shared_ptr<EResource>>> ResourceSet::getResources() const
{
    return resources_.get();
}

std::shared_ptr<EList<std::shared_ptr<EResource>>> ResourceSet::initResources()
{
    return std::shared_ptr<EList<std::shared_ptr<EResource>>>();
}
