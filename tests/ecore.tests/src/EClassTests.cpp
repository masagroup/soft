#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"


using namespace ecore;


BOOST_AUTO_TEST_SUITE( EClassTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    BOOST_CHECK( eClass );
}


BOOST_AUTO_TEST_SUITE_END()
