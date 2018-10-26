#include <boost/test/auto_unit_test.hpp>

#include "ecore/EcoreFactory.hpp"

using namespace ecore;

BOOST_AUTO_TEST_SUITE(EcoreFactoryTests)

BOOST_AUTO_TEST_CASE(Constructor)
{
    BOOST_CHECK( EcoreFactory::eInstance() );
}

BOOST_AUTO_TEST_SUITE_END()
