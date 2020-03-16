#include "ecore/EPackageResourceRegistry.hpp"
#include "ecore/impl/PackageResourceRegistry.hpp"

using namespace ecore;
using namespace ecore::impl;

std::shared_ptr<EPackageResourceRegistry> ecore::EPackageResourceRegistry::getInstance()
{
    static auto instance = std::make_shared<PackageResourceRegistry>();
    return std::static_pointer_cast<EPackageResourceRegistry>( instance );
}
