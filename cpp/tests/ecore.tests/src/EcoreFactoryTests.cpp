#include <boost/test/unit_test.hpp>

#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"

using namespace ecore;

BOOST_AUTO_TEST_SUITE(EcoreFactoryTests)

BOOST_AUTO_TEST_CASE(Constructor)
{
    BOOST_CHECK( EcoreFactory::eInstance() );
}

BOOST_AUTO_TEST_CASE( Getters )
{
    BOOST_CHECK_EQUAL( EcoreFactory::eInstance()->getEcorePackage(), EcorePackage::eInstance() );
}

BOOST_AUTO_TEST_SUITE_END()
