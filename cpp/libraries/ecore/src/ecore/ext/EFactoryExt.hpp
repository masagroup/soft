// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EFACTORYEXT_HPP
#define ECORE_EFACTORYEXT_HPP

#include "ecore/impl/EFactoryImpl.hpp"

namespace ecore::ext
{

    class EFactoryExt : public impl::EFactoryImpl
    {
    private:
        EFactoryExt( EFactoryExt const& ) = delete;
        EFactoryExt& operator=( EFactoryExt const& ) = delete;

    public:
        EFactoryExt();
        virtual ~EFactoryExt();

        //*********************************
        // Operations
        //*********************************
        virtual std::shared_ptr<ecore::EObject> create( const std::shared_ptr<ecore::EClass>& eClass ) const;

    };

}

#endif
