// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ENOTIFYINGLIST_HPP_
#define ECORE_ENOTIFYINGLIST_HPP_

#include "ecore/EList.hpp"

#include <memory>

namespace ecore
{
    class ENotifier;
    class ENotificationChain;
    class EStructuralFeature;

    template <typename T>
    class ENotifyingList : public EList<T>
    {
    public:
        virtual ~ENotifyingList() {}

        using EList::add;
        using EList::remove;

        virtual std::shared_ptr<ENotifier> getNotifier() const = 0;

        virtual std::shared_ptr<EStructuralFeature> getFeature() const = 0;

        virtual int getFeatureID() const = 0;

        virtual std::shared_ptr<ENotificationChain> add( const T& t, const std::shared_ptr<ENotificationChain>& notifications ) = 0;

        virtual std::shared_ptr<ENotificationChain> remove( const T& t, const std::shared_ptr<ENotificationChain>& notifications ) = 0;

        virtual std::shared_ptr<ENotificationChain> set( std::size_t index, const T& t, const std::shared_ptr<ENotificationChain>& notifications ) = 0;
    };
}



#endif /* ECORE_ENOTIFYINGLIST_HPP_ */
