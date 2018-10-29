// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_NOTIFICATION_HPP_
#define ECORE_NOTIFICATION_HPP_

#include "ecore/ENotification.hpp"
#include "ecore/ENotificationChain.hpp"

namespace ecore
{
    
    class Notification : public ENotification , public ENotificationChain , public std::enable_shared_from_this<Notification>
    {
    public:

        template< typename T >
        Notification( EventType type,
                       const std::shared_ptr<ENotifier>& notifier,
                       const std::shared_ptr<EStructuralFeature>& feature,
                       const T & oldValue,
                       const T & newValue ):
            eventType_( type ), notifier_( notifier ), feature_( feature ),
            oldValue_( oldValue ), newValue_( newValue ), position_( -1 )
        {
        }

        template< typename T >
        Notification( EventType type,
                       const std::shared_ptr<ENotifier>& notifier,
                       const std::shared_ptr<EStructuralFeature>& feature,
                       const T & oldValue,
                       const T & newValue,
                       std::size_t position ):
            eventType_( type ), notifier_( notifier ), feature_( feature ),
            oldValue_( oldValue ), newValue_( newValue ), position_( position )
        {
        }

        virtual ~Notification()
        {
        }

        ENotification::EventType getEventType() const
        {
            return eventType_;
        }

        std::shared_ptr<ENotifier> getNotifier() const
        {
            return notifier_;
        }

        std::shared_ptr<EStructuralFeature> getFeature() const
        {
            return feature_;
        }

        const boost::any& getOldValue() const
        {
            return oldValue_;
        }
        const boost::any& getNewValue() const
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
        std::shared_ptr<ENotifier> notifier_;
        std::shared_ptr<EStructuralFeature> feature_;
        boost::any oldValue_;
        boost::any newValue_;
        std::size_t position_;
        std::shared_ptr<ENotificationChain> next_;
    };
}



#endif /* ECORE_NOTIFICATION_HPP_ */
