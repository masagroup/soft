// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EPACKAGEREGISTRY_HPP_
#define ECORE_EPACKAGEREGISTRY_HPP_

#include "ecore/Exports.hpp"
#include <memory>
#include <string>

namespace ecore
{
    class EPackage;
    class EFactory;

    class ECORE_API EPackageRegistry
    {
    public:
        static std::shared_ptr<EPackageRegistry> getInstance();

        virtual ~EPackageRegistry() = default;

        virtual void registerPackage( const std::shared_ptr<EPackage>& package ) = 0;

        virtual void unregisterPackage( const std::shared_ptr<EPackage>& package ) = 0;

        virtual std::shared_ptr<EPackage> getPackage( const std::string& nsUri ) const  = 0;

        virtual std::shared_ptr<EFactory> getFactory( const std::string& nsUri ) const  = 0;
    };

} // namespace ecore

#endif
