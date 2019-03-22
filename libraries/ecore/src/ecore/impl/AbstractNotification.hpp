// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACT_NOTIFICATION_HPP_
#define ECORE_ABSTRACT_NOTIFICATION_HPP_

#include "ecore/Exports.hpp"
#include "ecore/Any.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/ENotificationChain.hpp"

namespace ecore::impl
{
    class ECORE_API AbstractNotification : public ENotification, public ENotificationChain, public std::enable_shared_from_this<AbstractNotification>
    {
    public:

        AbstractNotification( EventType type,
                              const Any& oldValue,
                              const Any& newValue,
                              std::size_t position = NO_INDEX )
            : eventType_( type )
            , oldValue_( oldValue )
            , newValue_( newValue )
            , position_( position )
            , next_()
        {
        }

        virtual ~AbstractNotification()
        {
        }

        ENotification::EventType getEventType() const
        {
            return eventType_;
        }

        const Any& getOldValue() const
        {
            return oldValue_;
        }
        const Any& getNewValue() const
        {
            return newValue_;
        }

        std::size_t getPosition() const
        {
            return position_;
        }

        bool merge( const std::shared_ptr<ENotification>& other );

        bool add( const std::shared_ptr<ENotification>& notification );

        void dispatch();

    protected:

        EventType eventType_;
        Any oldValue_;
        Any newValue_;
        std::size_t position_;
        std::shared_ptr<ENotificationChain> next_;
    };
}

#endif /* ECORE_ABSTRACT_NOTIFICATION_HPP_ */
