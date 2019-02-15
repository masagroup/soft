#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore\Stream.hpp"
#include "ecore\Uri.hpp"

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

BOOST_AUTO_TEST_SUITE( UriTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    Uri uri;
    BOOST_CHECK_EQUAL( uri.getScheme(), "" );
    BOOST_CHECK_EQUAL( uri.getHost(), "" );
    BOOST_CHECK_EQUAL( uri.getPort(), 0 );
    BOOST_CHECK_EQUAL( uri.getPath(), "" );
    BOOST_CHECK_EQUAL( uri.getQuery(), "" );
    BOOST_CHECK_EQUAL( uri.getFragment(), "" );
}

BOOST_AUTO_TEST_CASE( Constructor_Scheme )
{
    Uri uri{"http://"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
}

BOOST_AUTO_TEST_CASE( Constructor_Host )
{
    Uri uri{"http://host"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 0 );
}

BOOST_AUTO_TEST_CASE( Constructor_HostPort )
{
    Uri uri{"http://host:10020"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
}

BOOST_AUTO_TEST_CASE( Constructor_Path )
{
    Uri uri{"http://host:10020/path/path2"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
    BOOST_CHECK_EQUAL( uri.getPath(), "/path/path2" );
}

BOOST_AUTO_TEST_CASE( Constructor_RelativePath )
{
    Uri uri{"./path"};
    BOOST_CHECK_EQUAL( uri.getScheme(), "" );
    BOOST_CHECK_EQUAL( uri.getHost(), "" );
    BOOST_CHECK_EQUAL( uri.getPath(), "./path" );
}

BOOST_AUTO_TEST_CASE( Constructor_Query )
{
    Uri uri{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
    BOOST_CHECK_EQUAL( uri.getPath(), "/path/path2" );
    BOOST_CHECK_EQUAL( uri.getQuery(), "key1=foo&key2=&key3&=bar&=bar=" );
}

BOOST_AUTO_TEST_CASE( Constructor_QueryParameters )
{
    Uri uri{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    std::vector<std::pair<std::string, std::string>> expected
        = {std::make_pair( "key1", "foo" ), std::make_pair( "key2", "" ), std::make_pair( "key3", "" )};
    BOOST_CHECK_EQUAL( uri.getQueryParams(), expected );
}

BOOST_AUTO_TEST_CASE( Constructor_Fragment )
{
    {
        Uri uri{"http://host:10020/path/path2#fragment"};
        BOOST_CHECK_EQUAL( uri.getFragment(), "fragment" );
    }
    {
        Uri uri{"//#fragment"};
        BOOST_CHECK_EQUAL( uri.getScheme(), "" );
        BOOST_CHECK_EQUAL( uri.getAuthority(), "" );
        BOOST_CHECK_EQUAL( uri.getPath(), "" );
        BOOST_CHECK_EQUAL( uri.getFragment(), "fragment" );
    }
}


BOOST_AUTO_TEST_CASE( Equals_Empty )
{
    Uri uri1;
    Uri uri2;
    BOOST_CHECK_EQUAL( uri1, uri2 );
}

BOOST_AUTO_TEST_CASE( IsAbsolute )
{
    {
        Uri uri{"http://toto"};
        BOOST_CHECK( uri.isAbsolute() );
    }
    {
        Uri uri{"/toto"};
        BOOST_CHECK( !uri.isAbsolute() );
    }
}

BOOST_AUTO_TEST_CASE( IsOpaque )
{
    {
        Uri uri{"http://toto"};
        BOOST_CHECK( uri.isOpaque() );
    }
    {
        Uri uri{"http://toto/"};
        BOOST_CHECK( !uri.isOpaque() );
    }
}

BOOST_AUTO_TEST_CASE( Equals )
{
    Uri uri1{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    Uri uri2{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    BOOST_CHECK_EQUAL( uri1, uri2 );
}

BOOST_AUTO_TEST_CASE( Difference )
{
    Uri uri1{"http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar="};
    Uri uri2{};
    BOOST_CHECK_NE( uri1, uri2 );
}

BOOST_AUTO_TEST_CASE( Normalize )
{
    {
        Uri uri{"http://host:10020/path/../path2"};
        Uri expected{"http://host:10020/path2"};
        BOOST_CHECK_EQUAL( uri.normalize(), expected );
    }
    {
        Uri uri{"http://host:10020/./path"};
        Uri expected{"http://host:10020/path"};
        BOOST_CHECK_EQUAL( uri.normalize(), expected );
    }
    {
        Uri uri{"http://host:10020/path/./path2"};
        Uri expected{"http://host:10020/path/path2"};
        BOOST_CHECK_EQUAL( uri.normalize(), expected );
    }
}

BOOST_AUTO_TEST_CASE( Resolve )
{
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"http://host:10020/path2/"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), uri2 );
    }
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"../path2"};
        Uri expected{"http://host:10020/path2"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), expected );
    }
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"/path2"};
        Uri expected{"http://host:10020/path2"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), expected );
    }
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"./path2"};
        Uri expected{"http://host:10020/path/path2"};
        BOOST_CHECK_EQUAL( uri.resolve( uri2 ), expected );
    }
}

BOOST_AUTO_TEST_CASE( Relativize )
{
    {
        Uri uri{"http://host:10020/path/"};
        Uri uri2{"http://host:10020/path/path2"};
        Uri expected{"path2"};
        BOOST_CHECK_EQUAL( uri.relativize( uri2 ), expected );
    }
}

BOOST_AUTO_TEST_CASE( TrimFragment )
{
    {
        Uri uri{"http://host:10020/path/#fragment"};
        Uri expected{"http://host:10020/path/"};
        BOOST_CHECK_EQUAL( uri.trimFragment(), expected );
    }
}


BOOST_AUTO_TEST_SUITE_END()
