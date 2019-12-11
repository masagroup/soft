// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_URI_HPP_
#define ECORE_URI_HPP_

#include "ecore/Exports.hpp"
#include <string>
#include <vector>
#include <functional>

namespace ecore
{
    class ECORE_API URI
    {
    public:
        explicit URI();

        explicit URI( const std::string& str );

        const std::string& getScheme() const;
        
        const std::string& getUsername() const;
        
        const std::string& getPassword() const;
        
        /**
         * Get host part of URI. If host is an IPv6 address, square brackets will be
         * returned, for example: "[::1]".
         */
        const std::string& getHost() const;
        
        /**
         * Get host part of URI. If host is an IPv6 address, square brackets will not
         * be returned, for exmaple "::1"; otherwise it returns the same thing as
         * host().
         *
         * hostname() is what one needs to call if passing the host to any other tool
         * or API that connects to that host/port; e.g. getaddrinfo() only understands
         * IPv6 host without square brackets
         */
        std::string getHostName() const;

        uint16_t getPort() const;
        
        const std::string& getPath() const;

        const std::string& getQuery() const;

        const std::string& getFragment() const;

        std::string getAuthority() const;

        std::string toString() const;

        URI trimFragment() const;

        void setFragment(const std::string& fragment);

        std::string string() const;

        std::wstring wstring() const;

        std::u16string u16string() const;

        std::u32string u32string() const;

        /**
         * Get query parameters as key-value pairs.
         * e.g. for URI containing query string:  key1=foo&key2=&key3&=bar&=bar=
         * In returned list, there are 3 entries:
         *     "key1" => "foo"
         *     "key2" => ""
         *     "key3" => ""
         * Parts "=bar" and "=bar=" are ignored, as they are not valid query
         * parameters. "=bar" is missing parameter name, while "=bar=" has more than
         * one equal signs, we don't know which one is the delimiter for key and
         * value.
         *
         * Note, this method is not thread safe, it might update internal state, but
         * only the first call to this method update the state. After the first call
         * is finished, subsequent calls to this method are thread safe.
         *
         * @return  query parameter key-value pairs in a vector, each element is a
         *          pair of which the first element is parameter name and the second
         *          one is parameter value
         */
        const std::vector<std::pair<std::string, std::string>>& getQueryParams();

        bool operator==( const URI& other ) const;

        bool operator!=( const URI& other ) const;

        bool isAbsolute() const;

        bool isOpaque() const;

        bool isEmpty() const;

        URI normalize() const;

        URI resolve( const URI& uri ) const;

        URI resolve( const std::string& str ) const;

        URI relativize( const URI& uri ) const;

    private:
        // normalization
        static URI normalize( const URI& uri );
        
        // relativize
        static URI relativize( const URI& base, const URI& child );

        // resolve
        static URI resolve( const URI& base, const URI& child );
        
    private:
        std::string scheme_;
        std::string username_;
        std::string password_;
        std::string host_;
        uint16_t port_{0};
        std::string path_;
        std::string query_;
        std::string fragment_;
        std::vector<std::pair<std::string, std::string>> queryParams_;
    };

    /**
     * URI-escape a string.  Appends the result to the output string.
     *
     * Alphanumeric characters and other characters marked as "unreserved" in RFC
     * 3986 ( -_.~ ) are left unchanged.  In PATH mode, the forward slash (/) is
     * also left unchanged.  In QUERY mode, spaces are replaced by '+'.  All other
     * characters are percent-encoded.
     */
    enum class URIEscapeMode : unsigned char
    {
        ALL = 0,
        QUERY = 1,
        PATH = 2
    };

    template <typename InputIterator, typename OutputIterator>
    void uriEscape( InputIterator first,
                    InputIterator last,
                    OutputIterator out,
                    URIEscapeMode mode = URIEscapeMode::ALL );

    /**
     * Similar to uriEscape above, but returns the escaped string.
     */
    template <typename StringInput, typename StringOutput = StringInput>
    StringOutput uriEscape( const StringInput& str, URIEscapeMode mode = URIEscapeMode::ALL );

    /**
     * URI-unescape a string.  Appends the result to the output string.
     *
     * In QUERY mode, '+' are replaced by space.  %XX sequences are decoded if
     * XX is a valid hex sequence, otherwise we throw invalid_argument.
     */
    template <typename InputIterator, typename OutputIterator>
    void uriUnescape( InputIterator first,
                      InputIterator last,
                      OutputIterator out,
                      URIEscapeMode mode = URIEscapeMode::ALL );

    /**
     * Similar to uriUnescape above, but returns the unescaped string.
     */
    template <typename StringInput, typename StringOutput = StringInput>
    StringOutput uriUnescape( const StringInput& str, URIEscapeMode mode = URIEscapeMode::ALL );
    
} // namespace ecore

#include "ecore/URI.inl"

#endif
