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

#include <codecvt>
#include <locale>
#include <string>
#include <vector>

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

#if _MSC_VER >= 1900

    inline std::string utf16_to_utf8( std::u16string utf16_string )
    {
        std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
        auto p = reinterpret_cast<const int16_t*>( utf16_string.data() );
        return convert.to_bytes( p, p + utf16_string.size() );
    }

#else

    inline std::string utf16_to_utf8( std::u16string utf16_string )
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.to_bytes( utf16_string );
    }

#endif

    inline std::vector<std::string_view> split( const std::string& s, const std::string& token )
    {
        std::string_view sv = s;
        std::vector<std::string_view> result;
        std::size_t pos = 0;
        std::size_t start = 0;
        while( ( pos = sv.find( token, start ) ) != std::string::npos )
            result.emplace_back( sv.substr( start, pos ) );
        return result;
    }

    template < typename Range >
    std::string join(const Range& range, const std::string& sep) {
        std::string s;
        auto end = std::end(range);
        for (auto it = std::begin(range); it != end; ++it) {
            s += *it;
            if (it != end - 1)
                s += sep;
        }
        return s;
    }

} // namespace ecore::impl

#endif