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
    
    public:
        virtual ~EPackageExt();
    };

}

#endif
