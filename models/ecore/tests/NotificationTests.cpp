#include <boost/test/auto_unit_test.hpp>

#include "ecore/Notification.hpp"

using namespace ecore;

BOOST_AUTO_TEST_SUITE( NotificationTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    {
        auto notification = std::make_shared<Notification>( ENotification::ADD, nullptr, nullptr, 1, 2 );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::ADD );
        //BOOST_CHECK_EQUAL( notification->getNotifier(), nullptr );
        //BOOST_CHECK_EQUAL( notification->getFeature(), nullptr );
        BOOST_CHECK_EQUAL( boost::any_cast<int>(notification->getOldValue()), 1 );
        BOOST_CHECK_EQUAL( boost::any_cast<int>( notification->getNewValue()), 2 );
        BOOST_CHECK_EQUAL( notification->getPosition(), -1 );
    }
    {
        auto notification = std::make_shared<Notification>( ENotification::ADD, nullptr, nullptr, 1, 2 , 10 );
        BOOST_CHECK_EQUAL( notification->getEventType(), ENotification::ADD );
        //BOOST_CHECK_EQUAL( notification->getNotifier(), nullptr );
        //BOOST_CHECK_EQUAL( notification->getFeature(), nullptr );
        BOOST_CHECK_EQUAL( boost::any_cast<int>( notification->getOldValue() ), 1 );
        BOOST_CHECK_EQUAL( boost::any_cast<int>( notification->getNewValue() ), 2 );
        BOOST_CHECK_EQUAL( notification->getPosition(), 10 );
    }
}

BOOST_AUTO_TEST_SUITE_END()
