#include <boost/test/auto_unit_test.hpp>

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
    template<class T>
    bool out_to_stream( std::ostream& os, const Any& any_value )
    {
        try {
            T v = anyCast<T>(any_value);
            os << v;
            return true;
        }
        catch (BadAnyCast&) {
            return false;
        }
    }

    template<>
    bool out_to_stream<std::string>( std::ostream& os, const Any& any_value )
    {
        try {
            std::string v = std::move( anyCast<std::string>(any_value) );
            os << "'" << v << "'";
            return true;
        }
        catch (BadAnyCast&) {
            return false;
        }
    }

    ostream& operator <<( ostream& os, const Any& any_value )
    {
        //list all types you want to try
        if (!out_to_stream<int>( os, any_value ))
            if (!out_to_stream<double>( os, any_value ))
                if (!out_to_stream<bool>( os, any_value ))
                    if (!out_to_stream<std::string>( os, any_value ))
                        if (!out_to_stream<std::shared_ptr<MockObject>>( os, any_value ))
                            os << "{unknown}"; // all cast are failed, an unknown type of any
        return os;
    }

    template <typename T>
    ostream& operator <<( ostream& os, const std::vector<T>& v )
    {
        bool first = true;
        os << "[";
        for (auto b : v)
        {
            os << (first ? "" : ",") << b;
            first = false;
        }
        os << "]";
        return os;
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
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getOldValue()), 1 );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getNewValue()), 2 );
        BOOST_CHECK_EQUAL( notification->getPosition(), -1 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, 1, 2, 10 );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::ADD );
        BOOST_CHECK_EQUAL( notification->getNotifier(), notifier );
        BOOST_CHECK_EQUAL( notification->getFeature(), feature );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getOldValue()), 1 );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getNewValue()), 2 );
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
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, feature, 2, 3 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::SET );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getOldValue()), 1 );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getNewValue()), 3 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::SET, feature, 1, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::UNSET, feature, 2, 0 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::SET );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getOldValue()), 1 );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getNewValue()), 0 );
    }
    {
        auto notification = std::make_shared<Notification>( notifier, ENotification::UNSET, feature, 1, 0 );
        auto other = std::make_shared<Notification>( notifier, ENotification::SET, feature, 0, 2 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::SET );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getOldValue()), 1 );
        BOOST_CHECK_EQUAL( anyCast<int>(notification->getNewValue()), 2 );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj1, nullptr, 2 );
        auto other = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj2, nullptr, 2 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::REMOVE_MANY );
        auto oldValue = anyCast<std::vector<Any>>(notification->getOldValue());
        BOOST_CHECK( oldValue == std::vector<Any>( { obj1 , obj2 } ) );
        auto newValue = anyCast<std::vector<std::size_t>>(notification->getNewValue());
        BOOST_CHECK( newValue == std::vector<std::size_t>( { 2, 3 } ) );
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto obj3 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::REMOVE_MANY, feature, std::vector<Any>( { obj1 , obj2 } ), std::vector<std::size_t>( { 2, 3 } ) );
        auto other = std::make_shared<Notification>( notifier, ENotification::REMOVE, feature, obj3, nullptr, 2 );
        BOOST_CHECK( notification->merge( other ) );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::REMOVE_MANY );
        auto oldValue = std::move( anyCast<std::vector<Any>>(notification->getOldValue()) );
        BOOST_CHECK_EQUAL( oldValue, std::vector<Any>( { obj1 , obj2 , obj3 } ) );
        auto newValue = std::move( anyCast<std::vector<std::size_t>>(notification->getNewValue()) );
        BOOST_CHECK_EQUAL( newValue, std::vector<std::size_t>( { 2,3,4 } ) );
    }
}

BOOST_AUTO_TEST_CASE( Add )
{
    std::shared_ptr<MockObject> notifier = std::make_shared<MockObject>();
    std::shared_ptr<MockStructuralFeature> feature = std::make_shared<MockStructuralFeature>();
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
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, nullptr, obj1 );
        auto other = std::make_shared<Notification>( notifier, ENotification::ADD, feature, nullptr, obj2 );
        BOOST_CHECK( notification->add( other ) );
        MOCK_EXPECT( notifier->eNotify ).with( notification ).once();
        MOCK_EXPECT( notifier->eNotify ).with( other ).once();
        notification->dispatch();
    }
    {
        auto obj1 = std::make_shared<MockObject>();
        auto obj2 = std::make_shared<MockObject>();
        auto notification = std::make_shared<Notification>( notifier, ENotification::ADD, feature, nullptr, obj1 );
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
