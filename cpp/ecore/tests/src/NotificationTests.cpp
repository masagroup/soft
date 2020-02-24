#include <boost/test/unit_test.hpp>

#include "ecore/Stream.hpp"
#include "ecore/impl/Notification.hpp"
#include "ecore/tests/MockNotifier.hpp"
#include "ecore/tests/MockNotification.hpp"
#include "ecore/tests/MockStructuralFeature.hpp"

#include <type_traits>

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace std
{
    template <typename T>
    ostream& operator <<( ostream& os, const std::vector<T>& v )
    {
        return print_container(os,v);
    }

}
BOOST_AUTO_TEST_SUITE( NotificationTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, 1, 2 );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::ADD );
        BOOST_CHECK_EQUAL( notification->getNotifier(), notifier );
        BOOST_CHECK_EQUAL( notification->getFeature(), feature );
        BOOST_CHECK_EQUAL( notification->getOldValue(), 1 );
        BOOST_CHECK_EQUAL( notification->getNewValue(), 2 );
        BOOST_CHECK_EQUAL( notification->getPosition(), -1 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, 1, 2, 10 );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::ADD );
        BOOST_CHECK_EQUAL( notification->getNotifier(), notifier );
        BOOST_CHECK_EQUAL( notification->getFeature(), feature );
        BOOST_CHECK_EQUAL( notification->getOldValue(), 1 );
        BOOST_CHECK_EQUAL( notification->getNewValue(), 2 );
        BOOST_CHECK_EQUAL( notification->getPosition(), 10 );
    }
}

BOOST_AUTO_TEST_CASE( Dispatch )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, 1, 2 );
        MOCK_EXPECT( notifier->eNotify ).with( notification ).once();
        notification->dispatch();
    }
}

BOOST_AUTO_TEST_CASE( Merge )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    MOCK_EXPECT( feature->getFeatureID ).returns( 1 );
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, 1, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, 1, 2, 3 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::SET );
        BOOST_CHECK_EQUAL( notification->getOldValue(), 1 );
        BOOST_CHECK_EQUAL( notification->getNewValue(), 3 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::UNSET, feature, 2, 0 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::SET );
        BOOST_CHECK_EQUAL( notification->getOldValue(), 1 );
        BOOST_CHECK_EQUAL( notification->getNewValue(), 0 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::UNSET, feature, 1, 0 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, feature, 0, 2 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::SET );
        BOOST_CHECK_EQUAL( notification->getOldValue(), 1 );
        BOOST_CHECK_EQUAL( notification->getNewValue(), 2 );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj1, NO_VALUE, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj2, NO_VALUE, 2 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::REMOVE_MANY );
        BOOST_CHECK_EQUAL( notification->getOldValue(), std::vector<Any>( { obj1 , obj2 } ) );
        BOOST_CHECK_EQUAL( notification->getNewValue() , std::vector<std::size_t>( { 2, 3 } ) );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto obj3 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::REMOVE_MANY, feature, std::vector<Any>( { obj1 , obj2 } ), std::vector<std::size_t>( { 2, 3 } ) );
        auto other = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj3, NO_VALUE, 2 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::REMOVE_MANY );
        BOOST_CHECK_EQUAL( notification->getOldValue(), std::vector<Any>( { obj1 , obj2 , obj3 } ) );
        BOOST_CHECK_EQUAL( notification->getNewValue(), std::vector<std::size_t>( { 2,3,4 } ) );
    }
}

BOOST_AUTO_TEST_CASE( Add )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
    MOCK_EXPECT( feature->getFeatureID ).returns( 1 );
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        BOOST_CHECK( !notification->add( nullptr ) );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        BOOST_CHECK( !notification->add( other ) );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, NO_VALUE, obj1 );
        auto other = std::make_shared<Notification>( notifier, ENotification::ADD, feature, NO_VALUE, obj2 );
        BOOST_CHECK( notification->add( other ) );
        MOCK_EXPECT( notifier->eNotify ).with( notification ).once();
        MOCK_EXPECT( notifier->eNotify ).with( other ).once();
        notification->dispatch();
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, NO_VALUE, obj1 );
        auto other = std::make_shared<MockNotification>();
        auto otherNotifier = std::make_shared<MockNotifier>();
        MOCK_EXPECT( other->getEventType ).returns( ENotification::SET );
        MOCK_EXPECT( other->getNotifier ).returns( otherNotifier );
        MOCK_EXPECT( other->getFeature ).returns( feature );
        BOOST_CHECK( notification->add( other ) );
        MOCK_EXPECT( notifier->eNotify ).with( notification ).once();
        MOCK_EXPECT( otherNotifier->eNotify ).with( other ).once();
        notification->dispatch();
    }

}


BOOST_AUTO_TEST_SUITE_END()
