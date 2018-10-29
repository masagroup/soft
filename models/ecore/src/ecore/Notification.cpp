#include "Notification.hpp"
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#include "ecore/Notification.hpp"
#include "ecore/NotificationChain.hpp"
#include "ecore/ENotifier.hpp"

using namespace ecore;

bool Notification::merge( const std::shared_ptr<ENotification>& other )
{
    return false;
}

bool Notification::add( const std::shared_ptr<ENotification>& notification )
{
    if( !notification )
        return false;
    if( merge( notification ) )
        return false;
    if( !next_ )
    {
        if( auto n = std::dynamic_pointer_cast<Notification>( notification ) )
        {
            next_ = n;
            return true;
        }
        else
        {
            next_ = std::make_shared<NotificationChain>();
            return next_->add( notification );
        }
    }
    else
        return next_->add( notification );
}

void Notification::dispatch()
{
    if( notifier_ && eventType_ != -1 )
        notifier_->eNotify( std::enable_shared_from_this<Notification>::shared_from_this() );
    if( next_ )
        next_->dispatch();
}
