#include "ecore/impl/PackageRegistry.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EcorePackage.hpp"

using namespace ecore;
using namespace ecore::impl;

std::shared_ptr<PackageRegistry> PackageRegistry::createGlobalRegistry()
{
    auto registry = std::make_shared<PackageRegistry>();
    registry->registerPackage(EcorePackage::eInstance());
    return registry;
}

PackageRegistry::PackageRegistry()
{
}

PackageRegistry::PackageRegistry(const std::shared_ptr<EPackageRegistry>& delegate)
    :   delegate_(delegate)
{
}

PackageRegistry::~PackageRegistry()
{
    unregisterPackage( EcorePackage::eInstance() );
}

void PackageRegistry::registerPackage( const std::shared_ptr<EPackage>& package )
{
    packages_[package->getNsURI()] = package;
}

void PackageRegistry::unregisterPackage( const std::shared_ptr<EPackage>& package )
{
    packages_.extract( package->getNsURI() );
}

std::shared_ptr<EPackage> PackageRegistry::getPackage( const std::string& nsURI ) const
{
    auto it = packages_.find(nsURI);
    return it != packages_.end() ? it->second : ( delegate_ ? delegate_->getPackage(nsURI) : nullptr );
}

std::shared_ptr<EFactory> PackageRegistry::getFactory( const std::string& nsURI) const
{
    auto it = packages_.find(nsURI);
    return it != packages_.end() ? it->second->getEFactoryInstance() : (delegate_ ? delegate_->getFactory(nsURI) : nullptr);
}
