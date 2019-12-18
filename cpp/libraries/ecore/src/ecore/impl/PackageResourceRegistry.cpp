#include "ecore/impl/PackageResourceRegistry.hpp"
#include "ecore/impl/XMIResource.hpp"
#include "ecore/EPackage.hpp"

using namespace ecore;
using namespace ecore::impl;

std::shared_ptr<PackageResourceRegistry> ecore::impl::PackageResourceRegistry::getInstance()
{
    static std::shared_ptr<PackageResourceRegistry> instance = std::make_shared<PackageResourceRegistry>();
    return instance;
}

std::shared_ptr<EResource> PackageResourceRegistry::getResource( const std::shared_ptr<EPackage>& ePackage )
{
    auto itFound = packageToResources_.find( ePackage );
    if( itFound == packageToResources_.end() )
    {
        auto eResource = std::make_shared<XMIResource>( URI(ePackage->getNsURI()) );
        eResource->setThisPtr( eResource );
        eResource->getContents()->add( ePackage );
        return packageToResources_[ePackage] = eResource;
    }
    else
        return itFound->second;
}
