#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"


using namespace ecore;


BOOST_AUTO_TEST_SUITE( EAttributeTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
}


BOOST_AUTO_TEST_SUITE_END()
