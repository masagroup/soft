#include "ecore/EResourceFactoryRegistry.hpp"
#include "ecore/impl/ResourceFactoryRegistry.hpp"

using namespace ecore;
using namespace ecore::impl;

std::shared_ptr<EResourceFactoryRegistry> EResourceFactoryRegistry::getInstance()
{
    static auto instance = std::make_shared<ResourceFactoryRegistry>();
    return instance;
}
