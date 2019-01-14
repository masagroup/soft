// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ERESOURCE_SET_HPP_
#define ECORE_ERESOURCE_SET_HPP_

#include "ecore/ENotifier.hpp"

#include <memory>

namespace ecore
{
    class EResource;

    class EResourceSet
    {
    public:
        virtual ~EResourceSet() = default;

        virtual std::shared_ptr<EList<std::shared_ptr<EResource>>> getResources() const = 0;
    };

}

#endif
