#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/Uri.hpp"
#include "ecore/impl/FileUriHandler.hpp"
#include <iostream>

using namespace ecore;
using namespace ecore::impl;

BOOST_AUTO_TEST_SUITE( FileUriHandlerTests )

BOOST_AUTO_TEST_CASE( canHandle )
{
    FileUriHandler handler;
    BOOST_CHECK( handler.canHandle( Uri( "file://test.xml" ) ) );
    BOOST_CHECK( handler.canHandle( Uri( "/test.xml" ) ) );
    BOOST_CHECK( !handler.canHandle( Uri( "http://test.xml" ) ) );
}

BOOST_AUTO_TEST_CASE( InputStream_Read )
{
    FileUriHandler handler;
    std::unique_ptr<std::istream> is = handler.createInputStream( Uri("data/stream.read.txt") );
    char buff[256];
    is->read( buff, 256 );
    BOOST_REQUIRE_EQUAL( is->gcount(), 6 );
    buff[6] = 0;
    BOOST_CHECK_EQUAL( buff, "mytest" );
}

BOOST_AUTO_TEST_SUITE_END()
