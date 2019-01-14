// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_URI_HPP_
#error This file may only be included from folly/Uri.h
#endif

#include <functional>
#include <tuple>

namespace ecore
{
    namespace detail
    {
        using UriTuple = std::tuple<
            const std::string&,
            const std::string&,
            const std::string&,
            const std::string&,
            uint16_t,
            const std::string&,
            const std::string&,
            const std::string&>;

        inline UriTuple as_tuple( const Uri& k )
        {
            return UriTuple(
                k.scheme(),
                k.username(),
                k.password(),
                k.host(),
                k.port(),
                k.path(),
                k.query(),
                k.fragment() );
        }
    }

    

} 

namespace std
{
    template <>
    struct equal_to<ecore::Uri>
    {
        bool operator()( const ecore::Uri& a, const ecore::Uri& b ) const
        {
            return ecore::detail::as_tuple( a ) == ecore::detail::as_tuple( b );
        }
    };

}