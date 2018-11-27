// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTELIST_HPP_
#define ECORE_EOBJECTELIST_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/EUnsettableList.hpp"
#include "ecore/impl/AbstractArrayEList.hpp"
#include "ecore/impl/AbstractEObjectEList.hpp"



namespace ecore::impl
{

    template <typename T,bool containement = false, bool inverse = false, bool opposite = false , bool unset = false >
    class EObjectEList : public AbstractEObjectEList< AbstractArrayEList<T, typename std::conditional<unset, EUnsettableList<T>, ENotifyingList<T>>::type, true >
                                                    , containement
                                                    , inverse
                                                    , opposite>
    {
    public:
        using EList<T>::add;
        using ENotifyingList<T>::add;

        EObjectEList( const std::shared_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : AbstractEObjectEList( owner, featureID, inverseFeatureID )
        {
        }

        virtual ~EObjectEList()
        {
        }
    };
}



#endif /* ECORE_EOBJECTELIST_HPP_ */
