// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_URICONVERTER_HPP_
#define ECORE_URICONVERTER_HPP_

#include "ecore/Exports.hpp"
#include "ecore/EList.hpp"

#include <memory>
#include <iostream>

namespace ecore
{
    class URI;

    class URIHandler;

    class ECORE_API URIConverter
    {
    public:
        virtual ~URIConverter() = default;

        virtual std::unique_ptr<std::istream> createInputStream( const URI& uri ) const = 0;

        virtual std::unique_ptr<std::ostream> createOutputStream( const URI& uri ) const = 0;

        virtual URI normalize(const URI& uri) const = 0;

        virtual std::shared_ptr<URIHandler> getURIHandler( const URI& uri ) const = 0;

        virtual std::shared_ptr<const EList<std::shared_ptr<URIHandler>>> getURIHandlers() const = 0;
    };

} // namespace ecore

#endif