// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMPL_DIAGNOSTIC_HPP_
#define ECORE_IMPL_DIAGNOSTIC_HPP_

#include "ecore/EDiagnostic.hpp"

namespace ecore::impl
{
    class Diagnostic : public EDiagnostic
    {
    public:
        Diagnostic( const std::string& message, const std::string& location, int line , int column )
            : message_( message )
            , location_( location )
            , line_( line )
            , column_( column )
        {

        }

        virtual ~Diagnostic()
        {

        }

        virtual const std::string& getMessage() const
        {
            return message_;
        }

        virtual const std::string& getLocation() const
        {
            return location_;
        }

        virtual int getLine() const
        {
            return line_;
        }

        virtual int getColumn() const
        {
            return column_;
        }

    private:
        std::string message_;
        std::string location_;
        int line_;
        int column_;
    };
}

#endif