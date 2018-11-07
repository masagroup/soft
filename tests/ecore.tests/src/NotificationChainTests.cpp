#include <boost/test/auto_unit_test.hpp>

#include "ecore/NotificationChain.hpp"
#include "ecore/MockNotification.hpp"
#include "ecore/MockNotifier.hpp"
#include "ecore/MockStructuralFeature.hpp"

using namespace ecore;

BOOST_AUTO_TEST_SUITE( NotificationChainTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    BOOST_CHECK_NO_THROW( std::make_shared<NotificationChain>() );
}

BOOST_AUTO_TEST_CASE( AddAndDispatch)
{
    auto notifier = std::make_shared<MockNotifier>();
    auto feature = std::make_shared<MockStructuralFeature>();

    auto notification1 = std::make_shared<MockNotification>();
    MOCK_EXPECT( notification1->getEventType ).returns( ENotification::ADD );
    MOCK_EXPECT( notification1->getNotifier ).returns( notifier );
    
    auto notification2 = std::make_shared<MockNotification>();
    MOCK_EXPECT( notification2->getEventType ).returns( ENotification::ADD );
    MOCK_EXPECT( notification2->getNotifier ).returns( notifier );
    
    auto chain = std::make_shared<NotificationChain>();
    BOOST_CHECK( chain->add( notification1 ) );
    MOCK_EXPECT( notification1->merge ).with( notification2 ).returns( false );
    BOOST_CHECK( chain->add( notification2 ) );
    MOCK_EXPECT( notifier->eNotify ).with( notification1 );
    MOCK_EXPECT( notifier->eNotify ).with( notification2 );
    chain->dispatch();
}


BOOST_AUTO_TEST_SUITE_END()