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

BOOST_AUTO_TEST_CASE( InputStream )
{
    FileUriHandler handler;
    std::unique_ptr<EUriInputStream> is = handler.createInputStream( Uri() );
    BOOST_CHECK( is );
    BOOST_CHECK_EQUAL( is->getPosition() , 0 );
}

BOOST_AUTO_TEST_CASE( OutputStream )
{
    FileUriHandler handler;
    std::unique_ptr<EUriOutputStream> os = handler.createOutputStream( Uri() );
    BOOST_CHECK( os );
}

BOOST_AUTO_TEST_SUITE_END()
