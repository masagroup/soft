// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EURISTREAM_HPP_
#define ECORE_EURISTREAM_HPP_

#include <cstddef>

namespace ecore
{

    class EUriInputStream
    {
    public:
        virtual ~EUriInputStream() = default;

        virtual void reset() = 0;

        virtual std::size_t getSize() = 0;
        
        virtual std::size_t getPosition() = 0;

        virtual void read( const char* bytes, const std::size_t size ) = 0;
        
    };

    class EUriOutputStream
    {
    public:
        virtual ~EUriOutputStream() = default;

        virtual void write( const char* bytes, const std::size_t size ) = 0;

        virtual void flush() = 0;
    };


} // namespace ecore

#endif