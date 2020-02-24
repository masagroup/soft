#include <boost/test/unit_test.hpp>

#include "ecore/URI.hpp"
#include "ecore/impl/FileURIHandler.hpp"
#include <iostream>

using namespace ecore;
using namespace ecore::impl;

BOOST_AUTO_TEST_SUITE( FileURIHandlerTests )

BOOST_AUTO_TEST_CASE( canHandle )
{
    FileURIHandler handler;
    BOOST_CHECK( handler.canHandle( URI( "file://test.xml" ) ) );
    BOOST_CHECK( handler.canHandle( URI( "/test.xml" ) ) );
    BOOST_CHECK( !handler.canHandle( URI( "http://test.xml" ) ) );
}

BOOST_AUTO_TEST_CASE( InputStream_Read )
{
    FileURIHandler handler;
    std::unique_ptr<std::istream> is = handler.createInputStream( URI("data/stream.read.txt") );
    char buff[256];
    is->read( buff, 256 );
    BOOST_REQUIRE_EQUAL( is->gcount(), 6 );
    buff[6] = 0;
    BOOST_CHECK_EQUAL( buff, "mytest" );
}

BOOST_AUTO_TEST_SUITE_END()
