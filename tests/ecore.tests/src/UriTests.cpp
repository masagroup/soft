#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore\Stream.hpp"
#include "ecore\Uri.hpp"

using namespace ecore;


namespace std
{
    template <typename T, typename U >
    ostream& operator <<( ostream& os, const std::pair<T, U>& p )
    {
        return os << "(" << p.first << "," << p.second << ")";
    }

    template <typename T>
    ostream& operator <<( ostream& os, const std::vector<T>& v )
    {
        return print_container( os, v );
    }
}

BOOST_AUTO_TEST_SUITE( UriTests )

BOOST_AUTO_TEST_CASE( Uri_Constructor_Empty )
{
    BOOST_CHECK_THROW( Uri uri{ "" }, std::invalid_argument );
}

BOOST_AUTO_TEST_CASE( Uri_Constructor_Scheme )
{
    Uri uri{ "http://" };
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
}

BOOST_AUTO_TEST_CASE( Uri_Constructor_Host )
{
    Uri uri{ "http://host" };
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 0 );
}

BOOST_AUTO_TEST_CASE( Uri_Constructor_HostPort )
{
    Uri uri{ "http://host:10020" };
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
}

BOOST_AUTO_TEST_CASE( Uri_Constructor_Path )
{
    Uri uri{ "http://host:10020/path/path2" };
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
    BOOST_CHECK_EQUAL( uri.getPath(), "/path/path2" );
}

BOOST_AUTO_TEST_CASE( Uri_Constructor_Query )
{
    Uri uri{ "http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar=" };
    BOOST_CHECK_EQUAL( uri.getScheme(), "http" );
    BOOST_CHECK_EQUAL( uri.getHost(), "host" );
    BOOST_CHECK_EQUAL( uri.getPort(), 10020 );
    BOOST_CHECK_EQUAL( uri.getPath(), "/path/path2" );
    BOOST_CHECK_EQUAL( uri.getQuery(), "key1=foo&key2=&key3&=bar&=bar=" );
}

BOOST_AUTO_TEST_CASE( Uri_Constructor_QueryParameters )
{
    Uri uri{ "http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar=" };
    std::vector< std::pair<std::string, std::string> > expected = { std::make_pair( "key1","foo" ) , std::make_pair( "key2","" ), std::make_pair( "key3","" ) };
    BOOST_CHECK_EQUAL( uri.getQueryParams(), expected );
}

BOOST_AUTO_TEST_CASE( Uri_Equals )
{
    Uri uri1{ "http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar=" };
    Uri uri2{ "http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar=" };
    BOOST_CHECK_EQUAL( uri1, uri2 );
}

BOOST_AUTO_TEST_CASE( Uri_Difference )
{
    Uri uri1{ "http://host:10020/path/path2?key1=foo&key2=&key3&=bar&=bar=" };
    Uri uri2{};
    BOOST_CHECK_NE( uri1, uri2 );
}


BOOST_AUTO_TEST_SUITE_END()