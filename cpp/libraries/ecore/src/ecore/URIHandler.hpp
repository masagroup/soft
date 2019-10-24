// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_URIHANDLER_HPP_
#define ECORE_URIHANDLER_HPP_

#include "ecore/Exports.hpp"
#include <memory>
#include <iostream>

namespace ecore
{
    class URI;

    class ECORE_API URIHandler
    {
    public:
        virtual ~URIHandler() = default;

        virtual bool canHandle( const URI& uri ) const = 0;

        virtual std::unique_ptr<std::istream> createInputStream( const URI& uri ) const = 0;

        virtual std::unique_ptr<std::ostream> createOutputStream( const URI& uri ) const = 0;
    };

} // namespace ecore

#endif