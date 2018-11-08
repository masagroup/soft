// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#include "ecore/impl/Notification.hpp"
#include "ecore/impl/NotificationChain.hpp"
#include "ecore/impl/BasicEList.hpp"
#include "ecore/ENotifier.hpp"

using namespace ecore;
using namespace ecore::impl;

bool Notification::merge( const std::shared_ptr<ENotification>& notification )
{
    switch (eventType_)
    {
        case SET:
        case UNSET:
        {
            EventType notificationEventType = notification->getEventType();
            switch (notificationEventType)
            {
                case SET:
                case UNSET:
                {
                    if (notifier_ == notification->getNotifier() && feature_ == notification->getFeature())
                    {
                        newValue_ = notification->getNewValue();
                        if (notification->getEventType() == SET)
                            eventType_ = SET;
                        return true;
                    }
                }
            }
        }
        case REMOVE:
        {
            EventType notificationEventType = notification->getEventType();
            switch (notificationEventType)
            {
                case REMOVE:
                {
                    if (notifier_ == notification->getNotifier() && feature_ == notification->getFeature())
                    {
                        std::size_t originalPosition = getPosition();
                        std::size_t notificationPosition = notification->getPosition();
                        eventType_ = REMOVE_MANY;
                        std::vector<boost::any> removedValues;
                        if (originalPosition <= notificationPosition)
                        {
                            removedValues.push_back( oldValue_ );
                            removedValues.push_back( notification->getOldValue() );
                            newValue_ = std::vector<std::size_t>{ position_ = originalPosition, notificationPosition + 1 };
                        }
                        else
                        {
                            removedValues.push_back( notification->getOldValue() );
                            removedValues.push_back( oldValue_ );
                            newValue_ = std::vector<std::size_t>{ position_ = notificationPosition, originalPosition };
                        }
                        oldValue_ = removedValues;
                        return true;
                    }
                    break;
                }
            }
            break;
        }
        case REMOVE_MANY:
        {
            EventType notificationEventType = notification->getEventType();
            switch (notificationEventType)
            {
                case REMOVE:
                {
                    if (notifier_ == notification->getNotifier() && feature_ == notification->getFeature())
                    {
                        std::size_t notificationPosition = notification->getPosition();
                        std::vector<std::size_t> positions = std::move( boost::any_cast<std::vector<std::size_t>>(newValue_) );
                        std::vector<std::size_t> newPositions( positions.size() + 1 );

                        int index = 0;
                        while (index < positions.size())
                        {
                            std::size_t oldPosition = positions[index];
                            if (oldPosition <= notificationPosition)
                            {
                                newPositions[index++] = oldPosition;
                                ++notificationPosition;
                            }
                            else
                                break;
                        }

                        std::vector<boost::any> oldValue = std::move( boost::any_cast<std::vector<boost::any>>(oldValue_) );
                        oldValue.insert( oldValue.begin() + index, notification->getOldValue() );

                        newPositions[index] = notificationPosition;
                        while (++index < positions.size() + 1)
                            newPositions[index] = positions[index - 1];

                        oldValue_ = oldValue;
                        newValue_ = newPositions;
                        return true;
                    }
                    break;
                }
            }
            break;
        }
    }
    return false;
}

bool Notification::add( const std::shared_ptr<ENotification>& notification )
{
    if (!notification)
        return false;
    if (merge( notification ))
        return false;
    if (!next_)
    {
        if (auto n = std::dynamic_pointer_cast<Notification>(notification))
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
    if (notifier_ && eventType_ != -1)
        notifier_->eNotify( std::enable_shared_from_this<Notification>::shared_from_this() );
    if (next_)
        next_->dispatch();
}
