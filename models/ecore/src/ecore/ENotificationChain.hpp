// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_NOTIFICATION_CHAIN_HPP_
#define ECORE_NOTIFICATION_CHAIN_HPP_


#include <memory>

namespace ecore
{

    class ENotification;

    class ENotificationChain
    {
    public:
        ENotificationChain( ENotificationChain const& ) = delete;
        ENotificationChain& operator=( ENotificationChain const& ) = delete;

    public:
        ENotificationChain() = default;
        virtual ~ENotificationChain() {}

        virtual bool add( const std::shared_ptr<ENotification>& notification ) = 0;

        virtual void dispatch() = 0;

    };
}


#endif /* ECORE_NOTIFICATION_CHAIN_HPP_ */
