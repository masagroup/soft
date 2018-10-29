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

        ENotification() = default;

        virtual ~ENotification() {}

        virtual EventType getEventType() const = 0;
        
        virtual std::shared_ptr<ENotifier> getNotifier() const = 0;
        
        virtual std::shared_ptr<EStructuralFeature> getFeature() const = 0;
        
        virtual const boost::any& getOldValue() const = 0;
        
        virtual const boost::any& getNewValue() const = 0;
        
        virtual std::size_t getPosition() const = 0;
   
        virtual bool merge( const std::shared_ptr<ENotification>& other ) = 0;

    };
}
#endif
