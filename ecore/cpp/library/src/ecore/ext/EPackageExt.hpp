// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EPACKAGEEXT_HPP
#define ECORE_EPACKAGEEXT_HPP

#include "ecore/impl/EPackageImpl.hpp"

#include <memory>
#include <unordered_map>

namespace ecore::ext
{

    class EPackageExt : public impl::EPackageImpl
    {
    private:
        EPackageExt( EPackageExt const& ) = delete;
        EPackageExt& operator=( EPackageExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EPackageExt();
        void setThisPtr( const std::shared_ptr<EPackageExt>& thisPtr );

    public:
        virtual ~EPackageExt();

        virtual std::shared_ptr<ecore::EClassifier> getEClassifier( const std::string& name );

    private:
        class Adapter;
        std::unique_ptr<EAdapter> adapter_;
        std::unique_ptr<std::unordered_map<std::string, std::shared_ptr<ecore::EClassifier>>> nameToClassifierMap_;
    };

}

#endif
