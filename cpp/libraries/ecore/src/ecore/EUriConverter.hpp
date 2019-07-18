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

#include "ecore/Exports.hpp"
#include "ecore/EList.hpp"

#include <memory>
#include <iostream>

namespace ecore
{
    class Uri;

    class EUriHandler;

    class ECORE_API EUriConverter
    {
    public:
        virtual ~EUriConverter() = default;

        virtual std::unique_ptr<std::istream> createInputStream( const Uri& uri ) const = 0;

        virtual std::unique_ptr<std::ostream> createOutputStream( const Uri& uri ) const = 0;

        virtual std::shared_ptr<EUriHandler> getUriHandler( const Uri& uri ) const = 0;

        virtual std::shared_ptr<const EList<std::shared_ptr<EUriHandler>>> getUriHandlers() const = 0;
    };

} // namespace ecore

#endif