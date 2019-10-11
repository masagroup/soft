#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore\Stream.hpp"
#include "ecore\URI.hpp"

using namespace ecore;

namespace std
{
    template <typename T, typename U>
    ostream& operator<<( ostream& os, const std::pair<T, U>& p )
    {
        return os << "(" << p.first << "," << p.second << ")";
    }

    template <typename T>
    ostream& operator<<( ostream& os, const std::vector<T>& v )
    {
        return print_container( os, v );
    }
} // namespace std

BOOST_AUTO_TEST_SUITE(URITests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    URI uri;
    BOOST_CHECK_EQUAL( uri.getScheme(), "" );
    BOOST_CHECK_EQUAL( uri.getHost(), "" );
    BOOST_CHECK_EQUAL( uri.getPort(), 0 );
    BOOST_CHECK_EQUAL( uri.getPath(), "" );
    BOOST_CHECK_EQUAL( uri.getQuery(), "" );
    BOOST_CHECK_EQUAL( uri.getFragment(), "" );
}

BOOST_AUTO_TEST_CASE( Constructor_Scheme )
{
    URI uri{"http://"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
}

BOOST_AUTO_TEST_CASE( Constructor_Host )
{
    URI uri{"http://host"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 0 );
}

BOOST_AUTO_TEST_CASE( Constructor_HostPort )
{
    URI uri{"http://host:10020"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
}

BOOST_AUTO_TEST_CASE( Constructor_Path )
{
    URI uri{"http://host:10020/path/path2"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
    BOOST_CHECK_EQUAL( uri.getPath(), "/path/path2" );
}

BOOST_AUTO_TEST_CASE( Constructor_RelativePath )
{
    URI uri{"./path"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "" );
    BOOST_CHECK_EQUAL( uri.getHost(), "" );
    BOOST_CHECK_EQUAL( uri.getPath(), "./path" );
}

BOOST_AUTO_TEST_CASE( Constructor_Query )
{
    URI uri{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
    BOOST_CHECK_EQUAL( uri.getPath(), "/path/path2" );
    BOOST_CHECK_EQUAL( uri.getQuery(), "key1=foo&key2=&key3&=bar&=bar=" );
}

BOOST_AUTO_TEST_CASE( Constructor_QueryParameters )
{
    URI uri{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    std::vector<std::pair<std::string, std::string>> expected
        = {std::make_pair( "key1", "foo" ), std::make_pair( "key2", "" ), std::make_pair( "key3", "" )};
    BOOST_CHECK_EQUAL( uri.getQueryParams(), expected );
}

BOOST_AUTO_TEST_CASE( Constructor_Fragment )
{
    {
        URI uri{"http://host:10020/path/path2#fragment"};
        BOOST_CHECK_EQUAL( uri.getFragment(), "fragment" );
    }
    {
        URI uri{"//#fragment"};
        BOOST_CHECK_EQUAL( uri.getScheme(), "" );
        BOOST_CHECK_EQUAL( uri.getAuthority(), "" );
        BOOST_CHECK_EQUAL( uri.getPath(), "" );
        BOOST_CHECK_EQUAL( uri.getFragment(), "fragment" );
    }
}


BOOST_AUTO_TEST_CASE( Equals_Empty )
{
    URI uri1;
    URI uri2;
    BOOST_CHECK_EQUAL( uri1, uri2 );
}

BOOST_AUTO_TEST_CASE( IsAbsolute )
{
    {
        URI uri{"http://toto"};
        BOOST_CHECK( uri.isAbsolute() );
    }
    {
        URI uri{"/toto"};
        BOOST_CHECK( !uri.isAbsolute() );
    }
}

BOOST_AUTO_TEST_CASE( IsOpaque )
{
    {
        URI uri{"http://toto"};
        BOOST_CHECK( uri.isOpaque() );
    }
    {
        URI uri{"http://toto/"};
        BOOST_CHECK( !uri.isOpaque() );
    }
}

BOOST_AUTO_TEST_CASE( Equals )
{
    URI uri1{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    URI uri2{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    BOOST_CHECK_EQUAL( uri1, uri2 );
}

BOOST_AUTO_TEST_CASE( Difference )
{
    URI uri1{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    URI uri2{};
    BOOST_CHECK_NE( uri1, uri2 );
}

BOOST_AUTO_TEST_CASE( Normalize )
{
    {
        URI uri{"http://host:10020/path/../path2"};
        URI expected{"http://host:10020/path2"};
        BOOST_CHECK_EQUAL( uri.normalize(), expected );
    }
    {
        URI uri{"http://host:10020/./path"};
        URI expected{"http://host:10020/path"};
        BOOST_CHECK_EQUAL( uri.normalize(), expected );
    }
    {
        URI uri{"http://host:10020/path/./path2"};
        URI expected{"http://host:10020/path/path2"};
        BOOST_CHECK_EQUAL( uri.normalize(), expected );
    }
}

BOOST_AUTO_TEST_CASE( Resolve )
{
    {
        URI uri{"http://host:10020/path/"};
        URI uri2{"http://host:10020/path2/"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), uri2 );
    }
    {
        URI uri{"http://host:10020/path/"};
        URI uri2{"../path2"};
        URI expected{"http://host:10020/path2"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), expected );
    }
    {
        URI uri{"http://host:10020/path/"};
        URI uri2{"/path2"};
        URI expected{"http://host:10020/path2"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), expected );
    }
    {
        URI uri{"http://host:10020/path/"};
        URI uri2{"./path2"};
        URI expected{"http://host:10020/path/path2"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), expected );
    }
}

BOOST_AUTO_TEST_CASE( Relativize )
{
    {
        URI uri{"http://host:10020/path/"};
        URI uri2{"http://host:10020/path/path2"};
        URI expected{"path2"};
        BOOST_CHECK_EQUAL( uri.relativize( uri2 ), expected );
    }
}

BOOST_AUTO_TEST_CASE( TrimFragment )
{
    {
        URI uri{"http://host:10020/path/#fragment"};
        URI expected{"http://host:10020/path/"};
        BOOST_CHECK_EQUAL( uri.trimFragment(), expected );
    }
}


BOOST_AUTO_TEST_SUITE_END()
