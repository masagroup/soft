// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_PACKAGEREGISTRY_HPP_
#define ECORE_PACKAGEREGISTRY_HPP_

#include "ecore/Exports.hpp"
#include "ecore/EPackageRegistry.hpp"
#include <unordered_map>

namespace ecore::impl
{
    class ECORE_API PackageRegistry : public EPackageRegistry
    {
    public:
        PackageRegistry();

        virtual ~PackageRegistry();

        // Inherited via EPackageRegistry
        virtual void registerPackage( const std::shared_ptr<EPackage>& package ) override;
        virtual void unregisterPackage( const std::shared_ptr<EPackage>& package ) override;
        virtual std::shared_ptr<EPackage> getPackage( const std::string& nsUri ) const override;
        virtual std::shared_ptr<EFactory> getFactory( const std::string& nsUri ) const override;

    private:
        std::unordered_map<std::string, std::shared_ptr<EPackage>> packages_;
    };

} // namespace ecore

#endif
