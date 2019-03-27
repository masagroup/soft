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
#include "ecore/EUriConverter.hpp"

namespace ecore::impl
{
    class ECORE_API UriConverter : public EUriConverter
    {
    public:
        UriConverter();

        virtual ~UriConverter();

        virtual std::unique_ptr<std::istream> createInputStream( const Uri& uri ) const;

        virtual std::unique_ptr<std::ostream> createOutputStream( const Uri& uri ) const;

        virtual std::shared_ptr<EUriHandler> getUriHandler( const Uri& uri ) const;

        virtual std::shared_ptr<const EList<std::shared_ptr<EUriHandler>>> getUriHandlers() const;

    private:
        std::shared_ptr<const EList<std::shared_ptr<EUriHandler>>> uriHandlers_;
    };

} // namespace ecore::impl

#endif
