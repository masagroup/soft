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
#include "ecore/impl/Proxy.hpp"


namespace ecore::impl
{

    template <typename T, bool containement = false, bool inverse = false, bool opposite = false, bool proxies = false, bool unset = false >
    class EObjectEList : public AbstractEObjectEList< typename std::conditional<unset, EUnsettableList<T>, ENotifyingList<T>>::type
                                                    , typename std::conditional<proxies, Proxy<T>, T>::type
                                                    , containement
                                                    , inverse
                                                    , opposite>
    {
        EObjectEList(const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID, std::false_type)
            : AbstractEObjectEList(owner, featureID, inverseFeatureID)
        {
        }

        EObjectEList(const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID, std::true_type)
            : AbstractEObjectEList([](const Proxy<T>& p) { return p.get(); }
                                     , [=](const T& v) { return Proxy<T>(owner, featureID_, v); }
                                     , owner
                                     , featureID
                                     , inverseFeatureID)
        {
        }

    public:
        using EList<T>::add;
        using ENotifyingList<T>::add;

        EObjectEList( const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : EObjectEList( owner, featureID, inverseFeatureID , std::integral_constant<bool,proxies>() )
        {
        }

        virtual ~EObjectEList()
        {
        }
    };
}



#endif /* ECORE_EOBJECTELIST_HPP_ */
