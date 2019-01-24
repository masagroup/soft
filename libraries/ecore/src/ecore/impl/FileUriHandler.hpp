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

#include "ecore/EUriHandler.hpp"

namespace ecore::impl
{
    
    class FileUriHandler
    {
    public:
        FileUriHandler();

        virtual ~FileUriHandler();

        virtual bool canHandle( const Uri& uri ) const;

        virtual std::unique_ptr<EUriInputStream> createInputStream( const Uri& uri ) const;

        virtual std::unique_ptr<EUriOutputStream> createOutputStream( const Uri& uri ) const;
    };

} // namespace ecore

#endif