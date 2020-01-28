// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ESTRUCTURALFEATUREINTERNAL_HPP
#define ECORE_EXT_ESTRUCTURALFEATUREINTERNAL_HPP

#include "ecore/impl/EObjectInternal.hpp"
#include "ecore/ext/EStructuralFeatureInternal.hpp"


namespace ecore::ext
{
    class EStructuralFeatureInternal : public impl::EObjectInternal
    {
    public:
        virtual ~EStructuralFeatureInternal() = default;

        virtual void setFeatureID( int newFeatureID ) = 0;
    };
}

#endif /* ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP */