// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ENOTIFIER_HPP_
#define ECORE_ENOTIFIER_HPP_

#include "ecore/Exports.hpp"
#include <memory>

namespace ecore
{
    template <typename T>
    class EList;

    class EAdapter;
    class ENotification;

    class ECORE_API ENotifier
    {
    public:
        virtual ~ENotifier() = default;

        /**
        * Returns list of the adapters associated with this notifier.
        * @return the adapters associated with this notifier.
        */
        virtual EList<EAdapter*>& eAdapters() const = 0;

        /**
        * Returns whether this notifier will deliver notifications to the adapters.
        * @return whether notifications will be delivered.
        * @see #eSetDeliver
        */
        virtual bool eDeliver() const = 0;

        /**
        * Sets whether this notifier will deliver notifications to the adapters.
        * @param deliver whether or not to deliver.
        * @see #eDeliver()
        */
        virtual void eSetDeliver( bool deliver ) = 0;

        /**
        * Notifies a change to a feature of this notifier as described by the notification.
        * The notifications will generally be {@link #eDeliver() delivered}
        * to the {@link #eAdapters adapters}
        * via {@link Adapter#notifyChanged Adapter.notifyChanged}.
        * @param notification a description of the change.
        */
        virtual void eNotify( const std::shared_ptr<ENotification>& notification ) = 0;
    };

}

#endif
