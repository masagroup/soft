#include "ecore/EPackageRegistry.hpp"
#include "ecore/impl/PackageRegistry.hpp"

using namespace ecore;
using namespace ecore::impl;

std::shared_ptr<EPackageRegistry> EPackageRegistry::getInstance()
{
    static auto instance = PackageRegistry::createGlobalRegistry();
    return instance;
}
