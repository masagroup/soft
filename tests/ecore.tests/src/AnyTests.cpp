#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/Any.hpp"
#include "ecore/tests/MockObject.hpp"

using namespace ecore;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( AnyTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    Any a;
    BOOST_CHECK( a.empty() );
}

BOOST_AUTO_TEST_SUITE_END()