// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ERESOURCEFACTORY_HPP_
#define ECORE_ERESOURCEFACTORY_HPP_

#include "ecore/Exports.hpp"
#include <memory>

namespace ecore
{
    class EResource;

    class Uri;

    class ECORE_API EResourceFactory
    {
    public:
        virtual ~EResourceFactory() = default;

        virtual std::shared_ptr<EResource> createResource( const Uri& uri ) const = 0;
    };

}
#endif // ECORE_ERESOURCEFACTORY_HPP_