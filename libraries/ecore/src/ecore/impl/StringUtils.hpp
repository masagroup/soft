// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMPL_STRING_UTILS_HPP_
#define ECORE_IMPL_STRING_UTILS_HPP_

#include <string>

namespace ecore::impl
{

    template <typename CharT>
    bool endWith( const std::basic_string<CharT>& text, const std::basic_string<CharT>& token )
    {
        if( text.size() >= token.size() && text.compare( text.size() - token.size(), token.size(), token ) == 0 )
            return true;
        else
            return false;
    }

    template <typename CharT>
    bool endWith( const std::basic_string<CharT>& text, const CharT* token )
    {
        return endWith( text, std::basic_string<CharT>( token ) );
    }

    template <typename CharT>
    bool startsWith( const std::basic_string<CharT>& text, const std::basic_string<CharT>& token )
    {
        if( text.length() < token.length() )
            return false;
        return ( text.compare( 0, token.length(), token ) == 0 );
    }

    template <typename CharT>
    bool startsWith( const std::basic_string<CharT>& text, const CharT* token )
    {
        return startsWith( text, std::basic_string<CharT>( token ) );
    }

} // namespace ecore::impl

#endif