// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************


#include "ecore/impl/AbstractNotification.hpp"
#include "ecore/impl/NotificationChain.hpp"
#include "ecore/Stream.hpp"
#include "ecore/ENotifier.hpp"

using namespace ecore;
using namespace ecore::impl;

bool AbstractNotification::merge( const std::shared_ptr<ENotification>& notification )
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
                    if (this->getNotifier() == notification->getNotifier() && this->getFeatureID() == notification->getFeatureID())
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
                    if ( this->getNotifier() == notification->getNotifier() && this->getFeatureID() == notification->getFeatureID())
                    {
                        std::size_t originalPosition = getPosition();
                        std::size_t notificationPosition = notification->getPosition();
                        eventType_ = REMOVE_MANY;
                        std::vector<Any> removedValues;
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
                    if ( this->getNotifier() == notification->getNotifier() && this->getFeatureID() == notification->getFeatureID())
                    {
                        std::size_t notificationPosition = notification->getPosition();
                        std::vector<std::size_t> positions = std::move( anyCast<std::vector<std::size_t>>(newValue_) );
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

                        std::vector<Any> oldValue = std::move( anyCast<std::vector<Any>>(oldValue_) );
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

bool AbstractNotification::add( const std::shared_ptr<ENotification>& notification )
{
    if (!notification)
        return false;
    if (merge( notification ))
        return false;
    if (!next_)
    {
        if (auto n = std::dynamic_pointer_cast<AbstractNotification>(notification))
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

void AbstractNotification::dispatch()
{
    auto notifier = getNotifier();
    if ( notifier && eventType_ != -1)
        notifier->eNotify( std::enable_shared_from_this<AbstractNotification>::shared_from_this() );
    if (next_)
        next_->dispatch();
}
