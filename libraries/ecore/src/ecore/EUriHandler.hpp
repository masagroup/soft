// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EURIHANDLER_HPP_
#define ECORE_EURIHANDLER_HPP_

#include <memory>
#include <iostream>

namespace ecore
{
    class Uri;

    class EUriHandler
    {
    public:
        virtual ~EUriHandler() = default;

        virtual bool canHandle( const Uri& uri ) const = 0;

        virtual std::unique_ptr<std::istream> createInputStream( const Uri& uri ) const = 0;

        virtual std::unique_ptr<std::ostream> createOutputStream( const Uri& uri ) const = 0;
    };

} // namespace ecore

#endif