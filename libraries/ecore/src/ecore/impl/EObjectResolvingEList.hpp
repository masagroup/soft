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
#include "ecore/impl/AbstractEList.hpp"
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
        static_assert( is_shared_ptr<T>::value,"EObjectResolvingEList requires std::shared_ptr" );
    public:
        using EList<T>::add;
        using ENotifyingList<T>::add;

        EObjectResolvingEList( const std::shared_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : AbstractEObjectEList( owner, featureID, inverseFeatureID )
        {
        }

        virtual ~EObjectResolvingEList()
        {
        }

    private:
        std::vector< Proxy<T> > v_;
    };

}

#endif
