// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ECLASSIFIEREXT_HPP
#define ECORE_EXT_ECLASSIFIEREXT_HPP

#include "ecore/impl/EClassifierImpl.hpp"

namespace ecore::ext
{
    template <typename... I>
    class EClassifierBaseExt : public ecore::impl::EClassifierBase<I...>
    {
    private:
        EClassifierBaseExt& operator=( EClassifierBaseExt const& ) = delete;

    protected:
        EClassifierBaseExt();

    public:
        virtual ~EClassifierBaseExt();

        virtual bool isInstance( const ecore::Any& object ) const;

    protected:
        virtual int initClassifierID();
    };

    typedef EClassifierBaseExt<EClassifier> EClassifierExt;
}

#include "ecore/ext/EClassifierExt.inl"

#endif