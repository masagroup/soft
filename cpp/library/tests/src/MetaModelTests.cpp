#include <boost/test/unit_test.hpp>
#include "library/LibraryPackage.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"

using namespace library;
using namespace ecore;

BOOST_AUTO_TEST_SUITE( ModelTests )

BOOST_AUTO_TEST_CASE( TestContainer )
{
    auto p = LibraryPackage::eInstance();
    auto a = p->getLendable_Copies();
    BOOST_CHECK_EQUAL( std::static_pointer_cast<EObject>(p->getLendable()), a->eContainer() );
}


BOOST_AUTO_TEST_SUITE_END()