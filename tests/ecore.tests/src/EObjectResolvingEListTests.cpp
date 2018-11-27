#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/impl/EObjectResolvingEList.hpp"
#include "ecore/tests/MockObject.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
}

BOOST_AUTO_TEST_SUITE( EObjectResolvingEListTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto object = std::make_shared<MockObject>();
    {
        EObjectResolvingEList< std::shared_ptr<EObject> > list( object, 0, 1 );
    }
    {
        EObjectResolvingEList< std::shared_ptr<EObject>, false, true, true> list( object, 0, 1 );
    }
    {
        EObjectResolvingEList< std::shared_ptr<EObject>, false, true, true> list( object, 0, 1 );
    }
    {
        EObjectResolvingEList< std::shared_ptr<EObject>, false, true, true, true> list( object, 0, 1 );
    }
}

BOOST_AUTO_TEST_SUITE_END()

