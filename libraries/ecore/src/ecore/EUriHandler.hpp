// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EURICONVERTER_HPP_
#define ECORE_EURICONVERTER_HPP_

#include "ecore/EList.hpp"

#include <iostream>

namespace ecore
{
    class Uri;

    class EUriHandler;

    class EUriHandler
    {
    public:
        virtual ~EUriHandler() = default;

        virtual bool canHandle( const Uri& uri ) const = 0;

        virtual std::istream createInputStream( const Uri& uri ) const = 0;

        virtual std::ostream createOutputStream( const Uri& uri ) const = 0;
    };

}

#endif