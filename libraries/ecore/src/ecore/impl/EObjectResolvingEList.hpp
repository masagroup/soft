// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTRESOLVINGELIST_HPP_
#define ECORE_EOBJECTRESOLVINGELIST_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/EUnsettableList.hpp"
#include "ecore/impl/AbstractArrayEList.hpp"
#include "ecore/impl/AbstractEObjectEList.hpp"
#include "ecore/impl/TypeTraits.hpp"

#ifdef SHOW_DELETION
#include <iostream>
#endif

namespace ecore::impl
{
    template <typename T, bool containement = false, bool inverse = false, bool opposite = false, bool unset = false >
    class EObjectResolvingEList : public AbstractEObjectEList< AbstractEList<T, typename std::conditional<unset, EUnsettableList<T>, ENotifyingList<T>>::type, true >
        , containement
        , inverse
        , opposite>
    {
    private:
        static_assert( is_shared_ptr<T>::value,"EObjectResolvingEList requires std::shared_ptr" );

    };

}

#endif
