// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECLASSIFIER_ECLASSIFIEREXT_HPP
#define ECORE_ECLASSIFIER_ECLASSIFIEREXT_HPP

#include "ecore/impl/EClassifierImpl.hpp"

namespace ecore::ext
{

    class EClassifierExt : public virtual impl::EClassifierImpl
    {
    private:
        EClassifierExt& operator=( EClassifierExt const& ) = delete;

    protected:
        EClassifierExt();

    public:
        virtual ~EClassifierExt();

        virtual bool isInstance( const ecore::Any& object ) const;

    protected:
        virtual int initClassifierID();
    };

}

#endif