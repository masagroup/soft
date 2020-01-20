// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EFACTORYEXT_HPP
#define ECORE_EXT_EFACTORYEXT_HPP

#include "ecore/impl/EFactoryImpl.hpp"

namespace ecore::ext
{

    template <typename... I>
    class EFactoryBaseExt : public ecore::impl::EFactoryBase<I...>
    {
    private:
        EFactoryBaseExt( EFactoryBaseExt const& ) = delete;
        EFactoryBaseExt& operator=( EFactoryBaseExt const& ) = delete;

    public:
        EFactoryBaseExt();
        virtual ~EFactoryBaseExt();

        //*********************************
        // Operations
        //*********************************
        virtual std::shared_ptr<ecore::EObject> create( const std::shared_ptr<ecore::EClass>& eClass ) const;

    };

    typedef EFactoryBaseExt<EFactory> EFactoryExt;
}


#include "ecore/ext/EFactoryExt.inl"

#endif
