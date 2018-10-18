// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ENOTIFICATION_HPP_
#define ECORE_ENOTIFICATION_HPP_

#include <memory>
#include <boost/any.hpp>

namespace ecore
{
    class ENotifier;
    class EStructuralFeature;

    class ENotification
    {
    public:

        enum EventType
        {
            CREATE,
            SET,
            UNSET,
            ADD,
            REMOVE,
            ADD_MANY,
            REMOVE_MANY,
            MOVE,
            REMOVING_ADAPTER,
            RESOLVE,
            EVENT_TYPE_COUNT
        };

        template< typename T >
        ENotification( EventType type, const std::shared_ptr<ENotifier>& notifier,
                       const std::shared_ptr<EStructuralFeature>& feature,
                       const T & oldValue,
                       const T & newValue ):
            eventType_( type ), notifier_( notifier ), feature_( feature ),
            oldValue_( oldValue ), newValue_( newValue )
        {
        }

        virtual ~ENotification()
        {
        }

        EventType getEventType()
        {
            return eventType_;
        }

        std::shared_ptr<ENotifier> getNotifier()
        {
            return notifier_.lock();
        }

        std::shared_ptr<EStructuralFeature> getFeature()
        {
            return feature_.lock();
        }

        const boost::any& getOldValue()
        {
            return oldValue_;
        }
        const boost::any& getNewValue()
        {
            return newValue_;
        }

    protected:

        EventType eventType_;
        std::weak_ptr<ENotifier> notifier_;
        std::weak_ptr<EStructuralFeature> feature_;
        boost::any oldValue_;
        boost::any newValue_;

    };
}
#endif
