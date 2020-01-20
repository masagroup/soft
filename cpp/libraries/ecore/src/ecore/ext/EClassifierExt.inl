// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ECLASSIFIEREXT_HPP
#error This file may only be included from EClassifierExt.hpp
#endif


#include "ecore/EList.hpp"
#include "ecore/EPackage.hpp"

namespace ecore::ext
{
    template <typename... I>
    EClassifierBaseExt<I...>::EClassifierBaseExt()
    {
    }

    template <typename... I>
    EClassifierBaseExt<I...>::~EClassifierBaseExt()
    {
    }

    template <typename... I>
    bool EClassifierBaseExt<I...>::isInstance(const Any& object) const
    {
        return getInstanceClass() && getInstanceClass()->operator==(object.type());
    }

    template <typename... I>
    int EClassifierBaseExt<I...>::initClassifierID()
    {
        return getEPackage() ? static_cast<int>(getEPackage()->getEClassifiers()->indexOf(getThisPtr())) : -1;
    }

}