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

#include <string>
#include <vector>

namespace ecore
{
    class Uri
    {
    public:
        explicit Uri();

        explicit Uri( const std::string& str );

        const std::string& getScheme() const
        {
            return scheme_;
        }
        const std::string& getUsername() const
        {
            return username_;
        }
        const std::string& getPassword() const
        {
            return password_;
        }
        /**
         * Get host part of URI. If host is an IPv6 address, square brackets will be
         * returned, for example: "[::1]".
         */
        const std::string& getHost() const
        {
            return host_;
        }
        /**
         * Get host part of URI. If host is an IPv6 address, square brackets will not
         * be returned, for exmaple "::1"; otherwise it returns the same thing as
         * host().
         *
         * hostname() is what one needs to call if passing the host to any other tool
         * or API that connects to that host/port; e.g. getaddrinfo() only understands
         * IPv6 host without square brackets
         */
        std::string getHostname() const;

        uint16_t getPort() const
        {
            return port_;
        }
        const std::string& getPath() const
        {
            return path_;
        }
        const std::string& getQuery() const
        {
            return query_;
        }
        const std::string& getFragment() const
        {
            return fragment_;
        }

        std::string getAuthority() const;

        std::string toString() const;

        std::string str() const
        {
            return toString();
        }

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

        bool operator==( const Uri& other ) const;

        bool operator!=( const Uri& other ) const;

        bool isAbsolute() const;

        bool isOpaque() const;

        Uri normalize() const;

        Uri resolve( const Uri& uri ) const;

        Uri resolve( const std::string& str ) const;

        Uri relativize( const Uri& uri ) const;

    private:
        // normalization
        static int needsNormalization( const std::string& path );
        static Uri normalize( const Uri& uri );
        static std::string normalize( const std::string& ps );

        static void split( std::string& path, std::vector<int>& segs );
        static void removeDots( std::string& path, std::vector<int>& segs );
        static void maybeAddLeadingDot( std::string& path, std::vector<int>& segs );
        static int join( std::string& path, std::vector<int>& segs );

        // relativize
        static Uri relativize( const Uri& base, const Uri& child );

        // resolve
        static Uri resolve( const Uri& base, const Uri& child );
        static std::string resolvePath( const std::string& base, const std::string& child, bool isAbsolute );

    private:
        std::string scheme_;
        std::string username_;
        std::string password_;
        std::string host_;
        bool hasAuthority_{false};
        uint16_t port_{0};
        std::string path_;
        std::string query_;
        std::string fragment_;
        std::vector<std::pair<std::string, std::string>> queryParams_;
    };

} // namespace ecore

#include "ecore/Uri.inl"

#endif
