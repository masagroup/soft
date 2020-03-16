// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EPACKAGERESOURCECEREGISTRY_HPP_
#define ECORE_EPACKAGERESOURCECEREGISTRY_HPP_

#include "ecore/Exports.hpp"
#include <memory>

namespace ecore
{
    class EResource;
    class EPackage;
} // namespace ecore

namespace ecore
{
    class ECORE_API EPackageResourceRegistry
    {
    public:
        static std::shared_ptr<EPackageResourceRegistry> getInstance();

        virtual ~EPackageResourceRegistry() = default;

        virtual std::shared_ptr<EResource> getResource( const std::shared_ptr<EPackage>& ePackage ) = 0;
    };

} // namespace ecore

#endif
