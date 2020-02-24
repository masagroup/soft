// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_FILEURIHANDLER_HPP_
#define ECORE_FILEURIHANDLER_HPP_

#include "ecore/Exports.hpp"
#include "ecore/URIHandler.hpp"

namespace ecore::impl
{
    
    class ECORE_API FileURIHandler : public URIHandler
    {
    public:
        FileURIHandler();

        virtual ~FileURIHandler();

        virtual bool canHandle( const URI& uri ) const;

        virtual std::unique_ptr<std::istream> createInputStream( const URI& uri ) const;

        virtual std::unique_ptr<std::ostream> createOutputStream( const URI& uri ) const;
    };

} // namespace ecore

#endif