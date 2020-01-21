// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EPACKAGEBASEEXT_HPP
#define ECORE_EXT_EPACKAGEBASEEXT_HPP

#include "ecore/impl/EPackageBase.hpp"

#include <memory>
#include <unordered_map>

namespace ecore::ext
{
    template<typename... I>
    class EPackageBaseExt : public ecore::impl::EPackageBase<I...>
    {
    private:
        EPackageBaseExt( EPackageBaseExt const& ) = delete;
        EPackageBaseExt& operator=( EPackageBaseExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EPackageBaseExt();
        void setThisPtr( const std::shared_ptr<EPackageBaseExt>& thisPtr );

    public:
        virtual ~EPackageBaseExt();

        virtual std::shared_ptr<ecore::EClassifier> getEClassifier( const std::string& name );

    private:
        class Adapter;
        std::unique_ptr<EAdapter> adapter_;
        std::unique_ptr<std::unordered_map<std::string, std::shared_ptr<ecore::EClassifier>>> nameToClassifierMap_;
    };

}

#include "ecore/ext/EPackageBaseExt.inl"

#endif // ECORE_EXT_EPACKAGEBASEEXT_HPP
