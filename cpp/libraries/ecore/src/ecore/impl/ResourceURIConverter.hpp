// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_RESOURCEURICONVERTER_HPP_
#define ECORE_RESOURCEURICONVERTER_HPP_

#include "ecore/Exports.hpp"
#include "ecore/URIConverter.hpp"

namespace ecore::impl
{
    class ECORE_API ResourceURIConverter : public URIConverter
    {
    public:
        ResourceURIConverter();

        virtual ~ResourceURIConverter();

        virtual std::unique_ptr<std::istream> createInputStream( const URI& uri ) const;

        virtual std::unique_ptr<std::ostream> createOutputStream( const URI& uri ) const;

        virtual std::shared_ptr<URIHandler> getURIHandler( const URI& uri ) const;

        virtual std::shared_ptr<const EList<std::shared_ptr<URIHandler>>> getURIHandlers() const;

    private:
        std::shared_ptr<const EList<std::shared_ptr<URIHandler>>> uriHandlers_;
    };

} // namespace ecore::impl

#endif
