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

#include "ecore/Exports.hpp"

#include <memory>

namespace ecore
{
    class Any;
    class ENotifier;
    class EStructuralFeature;

    class ECORE_API ENotification
    {
    public:

        static const std::size_t NO_INDEX = -1;

        static const int NO_FEATURE_ID = -1;

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

        virtual ~ENotification() {}

        /**
        * Returns the type of change that has occurred.
        * @return the type of change that has occurred.
        */
        virtual EventType getEventType() const = 0;
        
        /**
        * Returns the object affected by the change.
        * @return the object affected by the change.
        */
        virtual std::shared_ptr<ENotifier> getNotifier() const = 0;
        
        /**
        * Returns the object representing the feature of the notifier that has changed.
        * @return the feature that has changed.
        */
        virtual std::shared_ptr<EStructuralFeature> getFeature() const = 0;
        
        /**
        * Returns the numeric ID of the feature.
        * @return the feature ID of the feature that has changed.
        */
        virtual int getFeatureID() const = 0;

        /**
        * Returns the value of the notifier's feature before the change occurred.
        * For a list-based feature, this represents a value, or a list of values, removed from the list.
        * For a move, this represents the old position of the moved value.
        * @return the old value of the notifier's feature.
        */
        virtual const Any& getOldValue() const = 0;
        
        /**
        * Returns the value of the notifier's feature after the change occurred.
        * For a list-based feature, this represents a value, or a vector of values, added to the list,
        * an std::vector<std::size_t> containing the original index of each value in the list of values removed from the list (except for the case of a clear),
        * the value moved within the list,
        * or empty otherwise.
        * @return the new value of the notifier's feature.
        */
        virtual const Any& getNewValue() const = 0;
        
        /**
        * Returns the position within a list-based feature at which the change occurred.
        * It returns {@link #NO_INDEX} when not applicable.
        * @return the position at which the change occurred.
        */
        virtual std::size_t getPosition() const = 0;
   
        /**
        * Returns whether the notification can be and has been merged with this one.
        * @return whether the notification can be and has been merged with this one.
        */
        virtual bool merge( const std::shared_ptr<ENotification>& notification ) = 0;

    };
}
#endif
