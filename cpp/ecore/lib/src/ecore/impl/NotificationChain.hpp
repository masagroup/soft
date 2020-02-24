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

#pragma warning( push )
#pragma warning( disable : 4275 )

#include "ecore/Exports.hpp"
#include "ecore/ENotificationChain.hpp"

#include <memory>
#include <vector>

namespace ecore::impl
{
    class ECORE_API NotificationChain : public ENotificationChain , public std::enable_shared_from_this<NotificationChain>
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

#pragma warning( pop )

#endif /* ECORE_NOTIFICATIONCHAIN_HPP_ */
