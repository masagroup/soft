#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EAdapter.hpp"
#include "ecore/impl/EObjectEList.hpp"
#include "ecore/tests/MockAdapter.hpp"
#include "ecore/tests/MockObject.hpp"
#include "ecore/tests/MockClass.hpp"
#include "ecore/tests/MockList.hpp"
#include "ecore/tests/MockStructuralFeature.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    class AddFixture
    {
    public:
        AddFixture( bool notifications = false )
            : owner( new MockObject() )
            , object( new MockObject() )
        {
            MOCK_EXPECT( owner->eDeliver ).returns( notifications );
        }

        std::shared_ptr<MockObject> owner;
        std::shared_ptr<MockObject> object;
    };

    class AddFixtureNotifications : public AddFixture
    {
    public:
        AddFixtureNotifications()
            : AddFixture(true)
            , mockClass( new MockClass())
            , mockFeature( new MockStructuralFeature())
            , mockAdapters( new MockList<EAdapter*>() )
        {
            MOCK_EXPECT( mockClass->getEStructuralFeature_EInt ).with( 1 ).returns( mockFeature );
            MOCK_EXPECT( mockAdapters->empty ).returns( false );
            MOCK_EXPECT( owner->eClass ).returns( mockClass );
            MOCK_EXPECT( owner->eAdapters ).returns( boost::ref( *mockAdapters ) );
            MOCK_EXPECT( owner->eNotify ).with( [ = ]( const std::shared_ptr<ENotification>& n )
            {
                return n->getNotifier() == owner
                    && n->getFeature() == mockFeature
                    && n->getOldValue().empty()
                    && boost::any_cast<std::shared_ptr<EObject>>( n->getNewValue() ) == object
                    && n->getPosition() == 0;
            } ).once();
        }

        std::shared_ptr<MockClass> mockClass;
        std::shared_ptr<MockStructuralFeature> mockFeature;
        std::shared_ptr<MockList<EAdapter*>> mockAdapters;
    };

}

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
    {
        EObjectEList< std::shared_ptr<EObject>, false, true, true, true> list( object, 0, 1 );
    }
}

BOOST_FIXTURE_TEST_CASE( Unset , AddFixture )
{
    EObjectEList<std::shared_ptr<EObject>, false, false, false, true> list( owner, 1, 2 );
    BOOST_CHECK( !list.isSet() );
    BOOST_CHECK( list.add(object) );
    BOOST_CHECK( list.isSet() );
    list.unset();
    BOOST_CHECK( !list.isSet() );
}

BOOST_FIXTURE_TEST_CASE( Add_SimpleNoNotifications , AddFixture )
{
    EObjectEList<std::shared_ptr<EObject>> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_SimpleNotifications, AddFixtureNotifications )
{
    EObjectEList<std::shared_ptr<EObject>> list( owner, 1, 2 );
    BOOST_CHECK(list.add( object ));
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_InverseNoNotifications, AddFixture )
{
    MOCK_EXPECT( object->eInverseAdd ).with( owner, -2, nullptr ).once().returns( nullptr );

    EObjectEList<std::shared_ptr<EObject>, false, true, false> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_InverseNotifications, AddFixtureNotifications )
{
    MOCK_EXPECT( object->eInverseAdd ).with( owner, -2, nullptr ).once().returns( nullptr );
    EObjectEList<std::shared_ptr<EObject>, false, true, false> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}


BOOST_FIXTURE_TEST_CASE( Add_InverseOppositeNoNotifications , AddFixture )
{
    MOCK_EXPECT( object->eInverseAdd ).with( owner, 2, nullptr ).once().returns( nullptr );

    EObjectEList<std::shared_ptr<EObject>, false, true, true> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}

BOOST_FIXTURE_TEST_CASE( Add_InverseOppositeNotifications , AddFixtureNotifications )
{
    MOCK_EXPECT( object->eInverseAdd ).with( owner, 2, nullptr ).once().returns( nullptr );

    EObjectEList<std::shared_ptr<EObject>, false, true, true> list( owner, 1, 2 );
    BOOST_CHECK( list.add( object ) );
    BOOST_CHECK_EQUAL( list.size(), 1 );
    BOOST_CHECK_EQUAL( list.get( 0 ), object );
    BOOST_CHECK( !list.add( object ) );
}


BOOST_AUTO_TEST_SUITE_END()
