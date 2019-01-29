#include "ecore/EResourceFactoryRegistry.hpp"
#include "ecore/impl/ResourceFactoryRegistry.hpp"

using namespace ecore;
using namespace ecore::impl;

EResourceFactoryRegistry& EResourceFactoryRegistry::getInstance()
{
    static ResourceFactoryRegistry instance;
    return instance;
}
