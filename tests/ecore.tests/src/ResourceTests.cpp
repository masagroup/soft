#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/Any.hpp"
#include "ecore/Stream.hpp"
#include "ecore/Uri.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/impl/Resource.hpp"
#include "ecore/tests/MockAdapter.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;


namespace
{
    class NotificationsFixture
    {
    public:
        NotificationsFixture()
            : resource( std::make_shared<Resource>() )
            , eAdapter( std::make_unique<MockAdapter>() )
        {
            BOOST_CHECK( resource );
            resource->setThisPtr( resource );
            resource->eAdapters().add( eAdapter.get() );
        }

        ~NotificationsFixture()
        {
            resource->eAdapters().remove( eAdapter.get() );
        }

    protected:
        std::shared_ptr<Resource> resource;
        std::unique_ptr<MockAdapter> eAdapter;
    };
}

BOOST_AUTO_TEST_SUITE( ResourceTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto resource = std::make_shared<Resource>();
    resource->setThisPtr( resource );
}

BOOST_AUTO_TEST_CASE( Attribute_Uri )
{
    auto resource = std::make_shared<Resource>();
    auto uri = Uri( "http://host:10020" );
    resource->setUri(  uri );
    BOOST_CHECK_EQUAL( resource->getUri(), uri );
}

BOOST_FIXTURE_TEST_CASE( Attribute_Uri_Notifications , NotificationsFixture  )
{
    auto uri = Uri( "http://host:10020" );

    MOCK_EXPECT( eAdapter->notifyChanged ).with( [ = ]( const std::shared_ptr<ENotification>& n )
    {
        return n->getNotifier() == resource
            && n->getFeatureID() == EResource::RESOURCE__URI
            && n->getOldValue() == Uri()
            && n->getNewValue() == uri
            && n->getPosition() == -1;
    } ).once();

    resource->setUri( uri );
    BOOST_CHECK_EQUAL( resource->getUri(), uri );
}

BOOST_AUTO_TEST_SUITE_END()