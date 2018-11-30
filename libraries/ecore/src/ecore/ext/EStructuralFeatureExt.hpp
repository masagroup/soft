// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP
#define ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP

#include "ecore/ext/ETypedElementExt.hpp"
#include "ecore/impl/EStructuralFeatureImpl.hpp"

namespace ecore::ext
{
    class EStructuralFeatureExt : public virtual ETypedElementExt, public virtual impl::EStructuralFeatureImpl
    {
    private:
        EStructuralFeatureExt& operator=( EStructuralFeatureExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EStructuralFeatureExt();

    public:
        virtual ~EStructuralFeatureExt();
    };

}

#endif /* ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP */