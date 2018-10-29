// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#include "ecore/NotificationChain.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/ENotifier.hpp"

using namespace ecore;

bool NotificationChain::add( const std::shared_ptr<ENotification>& notification )
{
    if (!notification )
        return false;
    for( auto n : notifications_ )
    {
        if( n && n->merge( notification ) )
            return false;
    }
    notifications_.push_back( notification );
    return true;
}

void NotificationChain::dispatch()
{
    for( auto notification : notifications_ )
    {
        auto notifier = notification->getNotifier();
        if( notifier &&  notification->getEventType() != -1 )
            notifier->eNotify( notification );
    }

}
