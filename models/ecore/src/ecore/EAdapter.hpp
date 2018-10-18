// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EADAPTER_HPP_
#define ECORE_EADAPTER_HPP_

#include <memory>

namespace ecore
{

    class ENotification;
    class ENotifier;
    
    class EAdapter
    {
    public:
        EAdapter( EAdapter const& ) = delete;
        EAdapter& operator=( EAdapter const& ) = delete;

    public:
        EAdapter() = default;

        virtual ~EAdapter();

        virtual void notifyChanged(const ENotification& notification) = 0;

        std::shared_ptr<ENotifier> getTarget();

        virtual void setTarget(const std::shared_ptr<ENotifier>& target);

    protected:
        std::weak_ptr<ENotifier> target_;
    };

}

#endif
