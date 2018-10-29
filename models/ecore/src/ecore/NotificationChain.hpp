// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_NOTIFICATIONCHAIN_HPP_
#define ECORE_NOTIFICATIONCHAIN_HPP_

#include "ecore/ENotificationChain.hpp"

#include <memory>
#include <vector>

namespace ecore
{
    class NotificationChain : public ENotificationChain , public std::enable_shared_from_this<ENotificationChain>
    {
    public:
        NotificationChain() = default;

        virtual ~NotificationChain() {}

        bool add( const std::shared_ptr<ENotification>& notification );

        void dispatch();

    private:
        std::vector< std::shared_ptr<ENotification> > notifications_;
    };
}



#endif /* ECORE_NOTIFICATIONCHAIN_HPP_ */
