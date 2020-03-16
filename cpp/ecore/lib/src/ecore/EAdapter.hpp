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

#include "ecore/Exports.hpp"

#include <memory>

namespace ecore
{

    class ENotification;
    class ENotifier;
    
    class ECORE_API EAdapter
    {
    public:
        virtual ~EAdapter() = default;

        virtual void notifyChanged(const std::shared_ptr<ENotification>& notification) = 0;

        virtual std::shared_ptr<ENotifier> getTarget() const = 0;

        virtual void setTarget(const std::shared_ptr<ENotifier>& target) = 0;
    };

}

#endif
