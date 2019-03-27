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

#include "ecore/Exports.hpp"

#include <memory>

namespace ecore
{

    class ENotification;

    class ECORE_API ENotificationChain
    {
    public:
        virtual ~ENotificationChain() {}

        /**
        * Adds a notification to the chain.
        * @return whether the notification was added.
        */
        virtual bool add( const std::shared_ptr<ENotification>& notification ) = 0;

        virtual void dispatch() = 0;

    };
}


#endif /* ECORE_NOTIFICATION_CHAIN_HPP_ */
