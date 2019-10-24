#include "ecore/impl/PackageRegistry.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EcorePackage.hpp"

using namespace ecore;
using namespace ecore::impl;

PackageRegistry::PackageRegistry()
{
    registerPackage( EcorePackage::eInstance() );
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
    return it != packages_.end() ? it->second : std::shared_ptr<EPackage>();
}

std::shared_ptr<EFactory> PackageRegistry::getFactory( const std::string& nsURI) const
{
    auto it = packages_.find(nsURI);
    return it != packages_.end() ? it->second->getEFactoryInstance() : std::shared_ptr<EFactory>();
}
