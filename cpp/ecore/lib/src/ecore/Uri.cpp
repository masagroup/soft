#include "ecore/URI.hpp"
#include "ecore/Assert.hpp"
#include "ecore/impl/StringUtils.hpp"

#include <boost/regex.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <sstream>
#include <utility>

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::detail;

namespace
{
    std::string submatch( const boost::smatch& m, int idx )
    {
        const auto& sub = m[idx];
        return std::string( sub.first, sub.second );
    }

    template <typename MakeItem, std::size_t... Index>
    constexpr auto make_array_with( MakeItem const& make, std::index_sequence<Index...> )
    {
        return std::array<decltype( make( 0 ) ), sizeof...( Index )>{{make( Index )...}};
    }

    template <std::size_t Size, typename MakeItem>
    constexpr auto make_array_with( MakeItem const& make )
    {
        return make_array_with( make, std::make_index_sequence<Size>{} );
    }

    struct string_table_hex_make_item
    {
        constexpr unsigned char operator()( std::size_t index ) const
        {
            // clang-format off
            return static_cast<unsigned char>(
                index >= '0' && index <= '9' ? index - '0' :
                index >= 'a' && index <= 'f' ? index - 'a' + 10 :
                index >= 'A' && index <= 'F' ? index - 'A' + 10 :
                16);
            // clang-format on
        }
    };

    struct string_table_uri_escape_make_item
    {
        //  0 = passthrough
        //  1 = unused
        //  2 = safe in path (/)
        //  3 = space (replace with '+' in query)
        //  4 = always percent-encode
        constexpr unsigned char operator()( std::size_t index ) const
        {
            // clang-format off
            return static_cast<unsigned char>(
                index >= '0' && index <= '9' ? 0 :
                index >= 'A' && index <= 'Z' ? 0 :
                index >= 'a' && index <= 'z' ? 0 :
                index == '-' ? 0 :
                index == '_' ? 0 :
                index == '.' ? 0 :
                index == '~' ? 0 :
                index == '/' ? 2 :
                index == ' ' ? 3 :
                4);
            // clang-format on
        }
    };

    // The following algorithm for path normalization avoids the creation of a
    // string object for each segment, as well as the use of a string buffer to
    // compute the final result, by using a single char array and editing it in
    // place.  The array is first split into segments, replacing each slash
    // with '\0' and creating a segment-index array, each element of which is
    // the index of the first char in the corresponding segment.  We then walk
    // through both arrays, removing ".", "..", and other segments as necessary
    // by setting their entries in the index array to -1.  Finally, the two
    // arrays are used to rejoin the segments and compute the final result.
    //
    // This code is based upon src/solaris/native/java/io/canonicalize_md.c

    // Check the given path to see if it might need normalization.  A path
    // might need normalization if it contains duplicate slashes, a "."
    // segment, or a ".." segment.  Return -1 if no further normalization is
    // possible, otherwise return the number of segments found.
    //
    // This method takes a string argument rather than a char array so that
    // this test can be performed without invoking path.toCharArray().
    //
    int needsNormalization( const std::string& path )
    {
        bool normal = true;
        int ns = 0;                       // Number of segments
        int end = (int)path.length() - 1; // Index of last char in path
        int p = 0;                        // Index of next char in path

        // Skip initial slashes
        while( p <= end )
        {
            if( path[p] != '/' )
                break;
            p++;
        }

        if( p > 1 )
            normal = false;

        // Scan segments

        while( p <= end )
        {
            // Looking at "." or ".." ?
            if( ( path[p] == '.' )
                && ( ( p == end )
                     || ( ( path[p + 1] == '/' )
                          || ( ( path[p + 1] == '.' ) && ( ( p + 1 == end ) || ( path[p + 2] == '/' ) ) ) ) ) )
            {
                normal = false;
            }
            ns++;

            // Find beginning of next segment
            while( p <= end )
            {
                if( path[p++] != '/' )
                    continue;

                // Skip redundant slashes
                while( p <= end )
                {
                    if( path[p] != '/' )
                        break;

                    normal = false;
                    p++;
                }
                break;
            }
        }
        return normal ? -1 : ns;
    }

    // Split the given path into segments, replacing slashes with nulls and
    // filling in the given segment-index array.
    //
    // Preconditions:
    //   segs.length == Number of segments in path
    //

    // Postconditions:
    //   All slashes in path replaced by '\0'
    //   segs[i] == Index of first char in segment i (0 <= i < segs.length)
    //
    void split( std::string& path, std::vector<int>& segs )
    {
        int end = (int)path.size() - 1; // Index of last char in path
        int p = 0;                      // Index of next char in path
        int i = 0;                      // Index of current segment

        // Skip initial slashes
        while( p <= end )
        {
            if( path[p] != '/' )
                break;
            path[p] = '\0';
            p++;
        }

        while( p <= end )
        {
            // Note start of segment
            segs[i++] = p++;

            // Find beginning of next segment
            while( p <= end )
            {
                if( path[p++] != '/' )
                    continue;

                path[p - 1] = '\0';

                // Skip redundant slashes
                while( p <= end )
                {
                    if( path[p] != '/' )
                        break;
                    path[p++] = '\0';
                }
                break;
            }
        }
        _ASSERT( i == segs.size() );
    }

    // Remove "." segments from the given path, and remove segment pairs
    // consisting of a non-".." segment followed by a ".." segment.
    //
    void removeDots( std::string& path, std::vector<int>& segs )
    {
        int ns = (int)segs.size();
        int end = (int)path.size() - 1;
        for( int i = 0; i < ns; i++ )
        {
            int dots = 0; // Number of dots found (0, 1, or 2)

            // Find next occurrence of "." or ".."
            do
            {
                int p = segs[i];
                if( path[p] == '.' )
                {
                    if( p == end )
                    {
                        dots = 1;
                        break;
                    }
                    else if( path[p + 1] == '\0' )
                    {
                        dots = 1;
                        break;
                    }
                    else if( ( path[p + 1] == '.' ) && ( ( p + 1 == end ) || ( path[p + 2] == '\0' ) ) )
                    {
                        dots = 2;
                        break;
                    }
                }
                i++;

            } while( i < ns );

            if( ( i > ns ) || ( dots == 0 ) )
                break;

            if( dots == 1 )
            {
                // Remove this occurrence of "."
                segs[i] = -1;
            }
            else
            {
                // If there is a preceding non-".." segment, remove both that
                // segment and this occurrence of ".."; otherwise, leave this
                // ".." segment as-is.
                int j;
                for( j = i - 1; j >= 0; j-- )
                {
                    if( segs[j] != -1 )
                        break;
                }

                if( j >= 0 )
                {
                    int q = segs[j];
                    if( !( ( path[q] == '.' ) && ( path[q + 1] == '.' ) && ( path[q + 2] == '\0' ) ) )
                    {
                        segs[i] = -1;
                        segs[j] = -1;
                    }
                }
            }
        }
    }

    // DEVIATION: If the normalized path is relative, and if the first
    // segment could be parsed as a scheme name, then prepend a "." segment
    //
    void maybeAddLeadingDot( std::string& path, std::vector<int>& segs )
    {
        if( path[0] == '\0' )
            // The path is absolute
            return;

        int ns = (int)segs.size();
        int f = 0; // Index of first segment
        while( f < ns )
        {
            if( segs[f] >= 0 )
                break;
            f++;
        }

        if( ( f >= ns ) || ( f == 0 ) )
            // The path is empty, or else the original first segment survived,
            // in which case we already know that no leading "." is needed
            return;

        int p = segs[f];
        while( ( p < path.size() ) && ( path[p] != ':' ) && ( path[p] != '\0' ) )
            p++;

        if( p >= path.size() || path[p] == '\0' )
            // No colon in first segment, so no "." needed
            return;

        // At this point we know that the first segment is unused,
        // hence we can insert a "." segment at that position
        path[0] = '.';
        path[1] = '\0';
        segs[0] = 0;
    }

    // Join the segments in the given path according to the given segment-index
    // array, ignoring those segments whose index entries have been set to -1,
    // and inserting slashes as needed.  Return the length of the resulting
    // path.
    //
    // Preconditions:
    //   segs[i] == -1 implies segment i is to be ignored
    //   path computed by split, as above, with '\0' having replaced '/'
    //
    // Postconditions:
    //   path[0] .. path[return value] == Resulting path
    //
    int join( std::string& path, std::vector<int>& segs )
    {
        int ns = (int)segs.size();      // Number of segments
        int end = (int)path.size() - 1; // Index of last char in path
        int p = 0;                      // Index of next path char to write
        if( path[p] == '\0' )
            // Restore initial slash for absolute paths
            path[p++] = '/';

        for( int i = 0; i < ns; i++ )
        {
            int q = segs[i]; // Current segment
            if( q == -1 )
                // Ignore this segment
                continue;

            if( p == q )
            {
                // We're already at this segment, so just skip to its end
                while( ( p <= end ) && ( path[p] != '\0' ) )
                    p++;

                if( p <= end )
                {
                    // Preserve trailing slash
                    path[p++] = '/';
                }
            }
            else if( p < q )
            {
                // Copy q down to p
                while( ( q <= end ) && ( path[q] != '\0' ) )
                    path[p++] = path[q++];

                if( q <= end )
                {
                    // Preserve trailing slash
                    path[p++] = '/';
                }
            }
            else
                _ASSERT( false );
        }
        return p;
    }

    std::string normalize( const std::string& ps )
    {
        // Does this path need normalization?

        int ns = needsNormalization( ps ); // Number of segments
        if( ns < 0 )
            // Nope -- just return it
            return ps;

        std::string path = ps;
        std::vector<int> segs( ns, 0 );
        split( path, segs );

        // Remove dots
        removeDots( path, segs );

        // Prevent scheme-name confusion
        maybeAddLeadingDot( path, segs );

        // Join the remaining segments and return the result
        int newSize = join( path, segs );
        path.resize( newSize );

        if( path == ps )
            // string was already normalized
            return ps;

        return path;
    }

    std::string resolvePath( const std::string& base, const std::string& child, bool isAbsolute )
    {
        int i = (int)base.find_last_of( '/' );
        int cn = (int)child.size();

        std::string path;
        if( cn == 0 )
        {
            if( i >= 0 )
                path = base.substr( 0, i + 1 );
        }
        else
        {
            path.reserve( base.size() + cn );
            if( i >= 0 )
                path.append( base.substr( 0, i + 1 ) );

            path.append( child );
        }
        return ::normalize( path );
    }

} // namespace


namespace ecore::detail {

    decltype(hexTable) hexTable = make_array_with<256>(string_table_hex_make_item{});

    decltype(uriEscapeTable) uriEscapeTable = make_array_with<256>(string_table_uri_escape_make_item{});
}


URI::URI()
{
}

URI::URI( const std::string& str )
{
    static const boost::regex uriRegex( "(([a-zA-Z][a-zA-Z0-9+.-]*):)?" // scheme:
                                      "([^?#]*)"                      // authority and path
                                      "(?:\\?([^#]*))?"               // ?query
                                      "(?:#(.*))?" );                 // #fragment
    static const boost::regex authorityAndPathRegex( "//([^/]*)(/.*)?" );

    boost::smatch  match;
    if( !boost::regex_match( str.begin(), str.end(), match, uriRegex ) )
        throw std::invalid_argument( "invalid URI :'" + str + "'" );

    scheme_ = submatch( match, 2 );
    std::transform( scheme_.begin(), scheme_.end(), scheme_.begin(), ::tolower );

    const std::string authorityAndPath( match[3].first, match[3].second );
    boost::smatch authorityAndPathMatch;
    if( !boost::regex_match(
            authorityAndPath.begin(), authorityAndPath.end(), authorityAndPathMatch, authorityAndPathRegex ) )
    {
        // Does not start with //, doesn't have authority
        path_ = authorityAndPath;
    }
    else
    {
        static const boost::regex authorityRegex(
            "(?:([^@:]*)(?::([^@]*))?@)?" // username, password
            "(\\[[^\\]]*\\]|[^\\[:]*)"    // host (IP-literal (e.g. '['+IPv6+']',dotted-IPv4, or named host)
            "(?::(\\d*))?" );             // port

        const auto authority = authorityAndPathMatch[1];
        boost::smatch authorityMatch;
        if( !boost::regex_match( authority.first, authority.second, authorityMatch, authorityRegex ) )
            throw std::invalid_argument( "invalid URI authority " + std::string( authority.first, authority.second ) );

        std::string port( authorityMatch[4].first, authorityMatch[4].second );
        if( !port.empty() )
            port_ = stoi( port );

        username_ = submatch( authorityMatch, 1 );
        password_ = submatch( authorityMatch, 2 );
        host_ = submatch( authorityMatch, 3 );
        path_ = submatch( authorityAndPathMatch, 2 );
    }

    query_ = submatch( match, 4 );
    fragment_ = submatch( match, 5 );
}

std::string URI::getAuthority() const
{
    std::stringstream s;
    if( !username_.empty() || !password_.empty() )
    {
        s << username_;

        if( !password_.empty() )
            s << ':' << password_;

        s << '@';
    }

    s << host_;
    if( port_ != 0 )
        s << ':' << port_;

    return s.str();
}

std::string URI::getHostName() const
{
    if( host_.size() > 0 && host_[0] == '[' )
    {
        // If it starts with '[', then it should end with ']', this is ensured by regex
        return host_.substr( 1, host_.size() - 2 );
    }
    return host_;
}

const std::vector<std::pair<std::string, std::string>>& URI::getQueryParams()
{
    if( !query_.empty() && queryParams_.empty() )
    {
        // Parse query string
        static const boost::regex queryParamRegex(
            "(^|&)"      /*start of query or start of parameter "&"*/
            "([^=&]*)=?" /*parameter name and "=" if value is expected*/
            "([^=&]*)"   /*parameter value*/
            "(?=(&|$))"  /*forward reference, next should be end of query or start of next parameter*/ );
        const boost::cregex_iterator paramBeginItr( query_.data(), query_.data() + query_.size(), queryParamRegex );
        boost::cregex_iterator paramEndItr;
        for( auto itr = paramBeginItr; itr != paramEndItr; ++itr )
        {
            if( itr->length( 2 ) == 0 )
            {
                // key is empty, ignore it
                continue;
            }
            queryParams_.emplace_back( std::string( ( *itr )[2].first, ( *itr )[2].second ), // parameter name
                                       std::string( ( *itr )[3].first, ( *itr )[3].second )  // parameter value
            );
        }
    }
    return queryParams_;
}

std::string URI::toString() const
{
    std::stringstream s;
    std::string auth = getAuthority();
    if (!scheme_.empty())
        s << scheme_ << ":";

    if (!auth.empty())
        s << "//" << auth;
    
    s << path_;

    if (!query_.empty())
        s << '?' << query_;

    if (!fragment_.empty())
        s << '#' << fragment_;
    return s.str();
}

URI URI::trimFragment() const
{
    URI uri;
    uri.scheme_ = scheme_;
    uri.username_ = username_;
    uri.password_ = password_;
    uri.host_ = host_;
    uri.port_ = port_;
    uri.path_ = path_;
    uri.query_ = query_;
    return uri;
}

std::string URI::string() const
{
    return toString();
}

std::wstring URI::wstring() const
{
    auto s = toString();
    return std::wstring( std::begin( s ), std::end( s ) );
}

std::u16string URI::u16string() const
{
    auto s = toString();
    return std::u16string( std::begin( s ), std::end( s ) );
}

std::u32string URI::u32string() const
{
    auto s = toString();
    return std::u32string( std::begin( s ), std::end( s ) );
}

URI URI::normalize() const
{
    return normalize( *this );
}

URI URI::resolve( const URI& uri ) const
{
    return resolve( *this, uri );
}

URI URI::resolve( const std::string& str ) const
{
    return resolve( *this, URI( str ) );
}

URI URI::relativize( const URI& uri ) const
{
    return relativize( *this, uri );
}

URI URI::normalize( const URI& u )
{
    if( u.isOpaque() || u.path_.empty() )
        return u;

    auto np = ::normalize( u.path_ );
    if( np == u.path_ )
        return u;

    URI v;
    v.scheme_ = u.scheme_;
    v.fragment_ = u.fragment_;
    v.username_ = u.username_;
    v.password_ = u.password_;
    v.host_ = u.host_;
    v.port_ = u.port_;
    v.path_ = np;
    v.query_ = u.query_;
    return v;
}


// If both URIs are hierarchical, their scheme and authority components are
// identical, and the base path is a prefix of the child's path, then
// return a relative URI that, when resolved against the base, yields the
// child; otherwise, return the child.
//
URI URI::relativize( const URI& base, const URI& child )
{
    // check if child if opaque first so that NPE is thrown

    // if child is null.

    if( child.isOpaque() || base.isOpaque() )
        return child;

    if( base.scheme_ != child.scheme_ || base.getAuthority() != child.getAuthority() )
        return child;

    std::string bp = ::normalize( base.path_ );
    std::string cp = ::normalize( child.path_ );
    if( bp != cp )
    {
        if( !endWith( bp, "/" ) )
            bp = bp + "/";

        if( !startsWith( cp, bp ) )
            return child;
    }

    URI v;
    v.path_ = cp.substr( bp.length() );
    v.query_ = child.query_;
    v.fragment_ = child.fragment_;
    return v;
}

URI URI::resolve( const URI& base, const URI& child )
{
    // check if child if opaque first so that NPE is thrown
    // if child is null.
    if( child.isOpaque() || base.isOpaque() )
        return child;

    // Reference to current document (lone fragment)
    if( ( child.scheme_.empty() ) && child.getAuthority().empty() && child.path_.empty() && !child.fragment_.empty()
        && ( child.query_.empty() ) )
    {
        if( !base.fragment_.empty() && child.fragment_ == base.fragment_ )
            return base;

        URI ru;
        ru.scheme_ = base.scheme_;
        ru.username_ = base.username_;
        ru.password_ = base.password_;
        ru.host_ = base.host_;
        ru.port_ = base.port_;
        ru.path_ = base.path_;
        ru.fragment_ = child.fragment_;
        ru.query_ = base.query_;
        return ru;
    }

    // Child is absolute
    if( !child.scheme_.empty() )
        return child;

    URI ru; // Resolved URI
    ru.scheme_ = base.scheme_;
    ru.query_ = child.query_;
    ru.fragment_ = child.fragment_;

    // Authority
    if( child.getAuthority().empty() )
    {
        ru.host_ = base.host_;
        ru.username_ = base.username_;
        ru.password_ = base.password_;
        ru.port_ = base.port_;
        std::string cp = child.path_.empty() ? "" : child.path_;
        if( ( cp.length() > 0 ) && ( cp[0] == '/' ) )
        {
            // Child path is absolute
            ru.path_ = child.path_;
        }
        else
        {
            // Resolve relative path
            ru.path_ = resolvePath( base.path_, cp, base.isAbsolute() );
        }
    }
    else
    {
        ru.host_ = child.host_;
        ru.username_ = child.username_;
        ru.password_ = child.password_;
        ru.port_ = child.port_;
        ru.path_ = child.path_;
    }
    // Recombine (nothing to do here)
    return ru;
}
