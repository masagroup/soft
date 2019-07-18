// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EDIAGNOSTIC_HPP_
#define ECORE_EDIAGNOSTIC_HPP_

#include <string>

namespace ecore
{
    class EDiagnostic
    {
    public:
        virtual ~EDiagnostic() = default;

        virtual const std::string& getMessage() const = 0;

        virtual const std::string& getLocation() const = 0;

        virtual int getLine() const = 0;

        virtual int getColumn() const = 0;
    };
}

#endif