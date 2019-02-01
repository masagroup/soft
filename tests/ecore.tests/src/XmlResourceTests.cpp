#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/impl/XmlResource.hpp"

using namespace ecore;
using namespace ecore::impl;

BOOST_AUTO_TEST_SUITE( XmlResourceTests )

BOOST_AUTO_TEST_CASE( Load )
{
    XmlResource resource( Uri( "simple.book.ecore" ) );
    resource.load();
}

BOOST_AUTO_TEST_SUITE_END()
