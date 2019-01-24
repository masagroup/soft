#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/Uri.hpp"
#include "ecore/impl/FileUriHandler.hpp"

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

BOOST_AUTO_TEST_CASE( InputStream_Invalid )
{
    FileUriHandler handler;
    BOOST_CHECK_THROW( handler.createInputStream( Uri() ) , std::exception );
}

BOOST_AUTO_TEST_CASE( OutputStream_Invalid )
{
    FileUriHandler handler;
    BOOST_CHECK_THROW( handler.createOutputStream( Uri() ), std::exception );
}

BOOST_AUTO_TEST_SUITE_END()
