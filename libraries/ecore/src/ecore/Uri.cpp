#include "ecore/Uri.hpp"

#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>

using namespace ecore;

namespace
{

    std::string submatch( const std::smatch& m, int idx )
    {
        const auto& sub = m[ idx ];
        return std::string( sub.first, sub.second );
    }

}

Uri::Uri( const std::string& str )
    : hasAuthority_( false )
    , port_( 0 )
{
    static const std::regex uriRegex(
        "([a-zA-Z][a-zA-Z0-9+.-]*):" // scheme:
        "([^?#]*)" // authority and path
        "(?:\\?([^#]*))?" // ?query
        "(?:#(.*))?" ); // #fragment
    static const std::regex authorityAndPathRegex( "//([^/]*)(/.*)?" );

    std::smatch match;
    if( !std::regex_match( str.begin(), str.end(), match, uriRegex ) ) 
        throw std::invalid_argument( "invalid URI :" + str );
 
    scheme_ = submatch( match, 1 );
    std::transform( scheme_.begin(), scheme_.end(), scheme_.begin(), ::tolower );

    const std::string authorityAndPath( match[ 2 ].first, match[ 2 ].second );
    std::smatch authorityAndPathMatch;
    if( !std::regex_match(
        authorityAndPath.begin(),
        authorityAndPath.end(),
        authorityAndPathMatch,
        authorityAndPathRegex ) )
    {
        // Does not start with //, doesn't have authority
        hasAuthority_ = false;
        path_ = authorityAndPath;
    }
    else
    {
        static const std::regex authorityRegex(
            "(?:([^@:]*)(?::([^@]*))?@)?" // username, password
            "(\\[[^\\]]*\\]|[^\\[:]*)" // host (IP-literal (e.g. '['+IPv6+']',
                                       // dotted-IPv4, or named host)
            "(?::(\\d*))?" ); // port

        const auto authority = authorityAndPathMatch[ 1 ];
        std::smatch authorityMatch;
        if( !std::regex_match(
            authority.first,
            authority.second,
            authorityMatch,
            authorityRegex ) )
            throw std::invalid_argument("invalid URI authority " + std::string( authority.first, authority.second ) );
       
        std::string port( authorityMatch[ 4 ].first, authorityMatch[ 4 ].second );
        if( !port.empty() )
            port_ = stoi(port);
       
        hasAuthority_ = true;
        username_ = submatch( authorityMatch, 1 );
        password_ = submatch( authorityMatch, 2 );
        host_ = submatch( authorityMatch, 3 );
        path_ = submatch( authorityAndPathMatch, 2 );
    }

    query_ = submatch( match, 3 );
    fragment_ = submatch( match, 4 );
}

std::string Uri::authority() const
{
    std::stringstream s;
    if( !username().empty() || !password().empty() )
    {
        s << username();
        
        if( !password().empty() )
            s << ':' << password();
        
        s << '@';
    }

    s << host();
    if( port() != 0 )
        s << ':' << port();
    
    return s.str();
}

std::string Uri::hostname() const
{
    if( host_.size() > 0 && host_[ 0 ] == '[' )
    {
        // If it starts with '[', then it should end with ']', this is ensured by
        // regex
        return host_.substr( 1, host_.size() - 2 );
    }
    return host_;
}

const std::vector<std::pair<std::string, std::string>>& Uri::getQueryParams()
{
    if( !query_.empty() && queryParams_.empty() )
    {
        // Parse query string
        static const std::regex queryParamRegex(
            "(^|&)" /*start of query or start of parameter "&"*/
            "([^=&]*)=?" /*parameter name and "=" if value is expected*/
            "([^=&]*)" /*parameter value*/
            "(?=(&|$))" /*forward reference, next should be end of query or
                        start of next parameter*/ );
        const std::cregex_iterator paramBeginItr(
            query_.data(), query_.data() + query_.size(), queryParamRegex );
        std::cregex_iterator paramEndItr;
        for( auto itr = paramBeginItr; itr != paramEndItr; ++itr )
        {
            if( itr->length( 2 ) == 0 )
            {
                // key is empty, ignore it
                continue;
            }
            queryParams_.emplace_back(
                std::string( ( *itr )[ 2 ].first, ( *itr )[ 2 ].second ), // parameter name
                std::string( ( *itr )[ 3 ].first, ( *itr )[ 3 ].second ) // parameter value
            );
        }
    }
    return queryParams_;
}

std::string Uri::toString() const
{
    std::stringstream s;
    if( hasAuthority_ )
    {
        s << scheme_ << "://";
        if( !password_.empty() )
        {
            s << username_ << ':' << password_ << '@';
        }
        else if( !username_.empty() )
        {
            s << username_ << '@';
        }
        s << host_;
        if( port_ != 0 )
            s << ':' << port_;

    }
    else
        s << scheme_ << ':';
    
    s << path_;
    if( !query_.empty() )
        s << '?' << query_;
    if( !fragment_.empty() )
        s << '#' << fragment_;
    
    return s.str();
}