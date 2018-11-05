#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EObjectEList.hpp"
#include "ecore/MockObject.hpp"

using namespace ecore;

BOOST_AUTO_TEST_SUITE( EObjectEListTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto object = std::make_shared<MockObject>();
    {
        EObjectEList< std::shared_ptr<EObject> > list(object,0,1);
    }
    {
        EObjectEList< std::shared_ptr<EObject>, false, true, true> list( object, 0, 1 );
    }
    {
        EObjectEList< std::shared_ptr<EObject>, false, true, true> list( object, 0, 1 );
    }
}

BOOST_AUTO_TEST_CASE( Add )
{
    auto object = std::make_shared<MockObject>();
    auto owner = std::make_shared<MockObject>();
    {
        std::shared_ptr<EList<std::shared_ptr<EObject>>> list = std::make_shared<EObjectEList<std::shared_ptr<EObject>>>( owner, 1, 2 );
        MOCK_EXPECT( owner->eDeliver ).once().returns( false );
        BOOST_CHECK( list->add( object ) );
        BOOST_CHECK_EQUAL( list->size() , 1 );
        BOOST_CHECK_EQUAL( list->get( 0 ), object );
        BOOST_CHECK( !list->add( object ) );
    }
    {
        std::shared_ptr<EList<std::shared_ptr<EObject>>> list = std::make_shared<EObjectEList<std::shared_ptr<EObject>>>( owner, 1, 2 );
        //MOCK_EXPECT( owner->eDeliver ).once().returns( true );
        //list->add( object );
    }
    
}

BOOST_AUTO_TEST_SUITE_END()
