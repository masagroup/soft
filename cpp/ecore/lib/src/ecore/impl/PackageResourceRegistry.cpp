#include "ecore/impl/PackageResourceRegistry.hpp"
#include "ecore/impl/XMIResource.hpp"
#include "ecore/EPackage.hpp"

using namespace ecore;
using namespace ecore::impl;

std::shared_ptr<EResource> PackageResourceRegistry::getResource( const std::shared_ptr<EPackage>& ePackage )
{
    auto itFound = packageToResources_.find( ePackage );
    if( itFound == packageToResources_.end() )
    {
        auto uri = URI( ePackage->getNsURI() );
        auto eResource = std::make_shared<XMIResource>( uri );
        eResource->setThisPtr( eResource );
        eResource->getContents()->add( ePackage );
        return packageToResources_[ePackage] = eResource;
    }
    else
        return itFound->second;
}
