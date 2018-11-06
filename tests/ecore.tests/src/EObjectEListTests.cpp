#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EObjectEList.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/MockAdapter.hpp"
#include "ecore/MockObject.hpp"
#include "ecore/MockClass.hpp"
#include "ecore/MockList.hpp"
#include "ecore/MockList.hpp"
#include "ecore/MockStructuralFeature.hpp"

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
    // simple add with no notifications
    {
        std::shared_ptr<EList<std::shared_ptr<EObject>>> list = std::make_shared<EObjectEList<std::shared_ptr<EObject>>>( owner, 1, 2 );
        MOCK_EXPECT( owner->eDeliver ).once().returns( false );
        BOOST_CHECK( list->add( object ) );
        BOOST_CHECK_EQUAL( list->size() , 1 );
        BOOST_CHECK_EQUAL( list->get( 0 ), object );
        BOOST_CHECK( !list->add( object ) );
    }
    // simple add with a notification
    {
        auto mockClass = std::make_shared<MockClass>();
        auto mockFeature = std::make_shared<MockStructuralFeature>();
        auto mockAdapters = std::make_shared<MockList<EAdapter*>>();
        MOCK_EXPECT( mockClass->getEStructuralFeature_EInt ).with( 1 ).returns( mockFeature );
        MOCK_EXPECT( mockAdapters->empty ).returns( false );
        MOCK_EXPECT( owner->eClass).returns(mockClass);
        MOCK_EXPECT( owner->eDeliver ).once().returns( true );
        MOCK_EXPECT( owner->eAdapters ).returns( boost::ref(*mockAdapters) );
        MOCK_EXPECT( owner->eNotify ).with( [ = ]( const std::shared_ptr<ENotification>& n )
        {
            return n->getNotifier() == owner
                && n->getFeature() == mockFeature
                && n->getOldValue().empty()
                && boost::any_cast<std::shared_ptr<EObject>>( n->getNewValue() ) == object
                && n->getPosition() == 0;
        } ).once();
        std::shared_ptr<EList<std::shared_ptr<EObject>>> list = std::make_shared<EObjectEList<std::shared_ptr<EObject>>>( owner, 1, 2 );
        BOOST_CHECK(list->add( object ));
        BOOST_CHECK( !list->add( object ) );
    }
    
}

BOOST_AUTO_TEST_SUITE_END()
