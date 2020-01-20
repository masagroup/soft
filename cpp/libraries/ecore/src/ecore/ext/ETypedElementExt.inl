// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ETYPEDELEMENTEXT_HPP
#error This file may only be included from ETypedElementExt.hpp
#endif

#include "ecore/Constants.hpp"

namespace ecore::ext
{
    template <typename... I>
    ETypedElementBaseExt<I...>::ETypedElementBaseExt()
    {
    }

    template <typename... I>
    ETypedElementBaseExt<I...>::~ETypedElementBaseExt()
    {
    }

    template <typename... I>
    bool ETypedElementBaseExt<I...>::isMany() const
    {
        int upper = getUpperBound();
        return upper > 1 || upper == UNBOUNDED_MULTIPLICITY;
    }

    template <typename... I>
    bool ETypedElementBaseExt<I...>::isRequired() const
    {
        int lower = getLowerBound();
        return lower >= 1;
    }
}