// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_PACKAGERESOURCECEREGISTRY_HPP_
#define ECORE_PACKAGERESOURCECEREGISTRY_HPP_

#include "ecore/Exports.hpp"
#include <memory>
#include <unordered_map>

namespace ecore
{
    class EResource;
    class EPackage;
}

namespace ecore::impl
{
    class ECORE_API PackageResourceRegistry
    {
    public:
        static std::shared_ptr<PackageResourceRegistry> getInstance();

        std::shared_ptr<EResource> getResource( const std::shared_ptr<EPackage>& ePackage);

    private:
        std::unordered_map < std::shared_ptr<EPackage>, std::shared_ptr<EResource>> packageToResources_;
    };

} // namespace ecore::impl

#endif
