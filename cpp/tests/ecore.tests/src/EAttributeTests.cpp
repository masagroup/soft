#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/Constants.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EList.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/tests/MockAdapter.hpp"

using namespace ecore;
using namespace ecore::tests;

namespace
{
    class AttributeNotificationsFixture
    {
    public:
        AttributeNotificationsFixture()
            : eAttribute( EcoreFactory::eInstance()->createEAttribute() )
            , eAdapter( std::make_unique<MockAdapter>() )
        {
            BOOST_CHECK( eAttribute );
            MOCK_EXPECT( eAdapter->setTarget ).with( eAttribute ).once();
            eAttribute->eAdapters().add( eAdapter.get() );
        }

        ~AttributeNotificationsFixture()
        {
            MOCK_EXPECT( eAdapter->setTarget ).with( nullptr ).once();
            eAttribute->eAdapters().remove( eAdapter.get() );
        }

    protected:
        std::shared_ptr<EAttribute> eAttribute;
        std::unique_ptr<MockAdapter> eAdapter;
    };
} // namespace

BOOST_AUTO_TEST_SUITE( EAttributeTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
}

BOOST_AUTO_TEST_CASE( Accessors_ENotifier )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->eDeliver(), true );
    eAttribute->eSetDeliver( false );
    BOOST_CHECK_EQUAL( eAttribute->eDeliver(), false );
}

BOOST_AUTO_TEST_CASE( Accessors_ENamedElement )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->getName(), "" );
    eAttribute->setName( "toto" );
    BOOST_CHECK_EQUAL( eAttribute->getName(), "toto" );
}

BOOST_FIXTURE_TEST_CASE( Accessors_ENamedElement_Notifications, AttributeNotificationsFixture )
{
    MOCK_EXPECT( eAdapter->notifyChanged )
        .with( [=]( const std::shared_ptr<ENotification>& n ) {
            return n->getNotifier() == eAttribute && n->getFeatureID() == EcorePackage::EATTRIBUTE__NAME
                   && n->getOldValue() == std::string() && n->getNewValue() == std::string( "toto" ) && n->getPosition() == -1;
        } )
        .once();

    eAttribute->setName( "toto" );
    BOOST_CHECK_EQUAL( eAttribute->getName(), "toto" );
}

BOOST_AUTO_TEST_CASE( Accessors_ETypedElement_Default )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->isOrdered(), true );
    BOOST_CHECK_EQUAL( eAttribute->isUnique(), true );
    BOOST_CHECK_EQUAL( eAttribute->getLowerBound(), 0 );
    BOOST_CHECK_EQUAL( eAttribute->getUpperBound(), 1 );
    BOOST_CHECK_EQUAL( eAttribute->isMany(), false );
    BOOST_CHECK_EQUAL( eAttribute->isRequired(), false );
}

BOOST_AUTO_TEST_CASE( Accessors_ETypedElement_Setters )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    eAttribute->setUpperBound( UNBOUNDED_MULTIPLICITY );
    BOOST_CHECK_EQUAL( eAttribute->isMany(), true );
    eAttribute->setLowerBound( 1 );
    BOOST_CHECK_EQUAL( eAttribute->isRequired(), true );
    eAttribute->setUnique( false );
    BOOST_CHECK_EQUAL( eAttribute->isUnique(), false );
    eAttribute->setOrdered( false );
    BOOST_CHECK_EQUAL( eAttribute->isOrdered(), false );
}

BOOST_FIXTURE_TEST_CASE( Accessors_ETypedElement_Setters_Notifications, AttributeNotificationsFixture )
{
    MOCK_EXPECT( eAdapter->notifyChanged )
        .with( [=]( const std::shared_ptr<ENotification>& n ) {
            return n->getNotifier() == eAttribute && n->getFeatureID() == EcorePackage::EATTRIBUTE__UPPER_BOUND && n->getOldValue() == 1
                   && n->getNewValue() == UNBOUNDED_MULTIPLICITY && n->getPosition() == -1;
        } )
        .once();
    eAttribute->setUpperBound( UNBOUNDED_MULTIPLICITY );

    MOCK_EXPECT( eAdapter->notifyChanged )
        .with( [=]( const std::shared_ptr<ENotification>& n ) {
            return n->getNotifier() == eAttribute && n->getFeatureID() == EcorePackage::EATTRIBUTE__LOWER_BOUND && n->getOldValue() == 0
                   && n->getNewValue() == 1 && n->getPosition() == -1;
        } )
        .once();
    eAttribute->setLowerBound( 1 );

    MOCK_EXPECT( eAdapter->notifyChanged )
        .with( [=]( const std::shared_ptr<ENotification>& n ) {
            return n->getNotifier() == eAttribute && n->getFeatureID() == EcorePackage::EATTRIBUTE__UNIQUE && n->getOldValue() == true
                   && n->getNewValue() == false && n->getPosition() == -1;
        } )
        .once();
    eAttribute->setUnique( false );

    MOCK_EXPECT( eAdapter->notifyChanged )
        .with( [=]( const std::shared_ptr<ENotification>& n ) {
            return n->getNotifier() == eAttribute && n->getFeatureID() == EcorePackage::EATTRIBUTE__ORDERED && n->getOldValue() == true
                   && n->getNewValue() == false && n->getPosition() == -1;
        } )
        .once();
    eAttribute->setOrdered( false );
}

BOOST_AUTO_TEST_CASE( Accessors_EStructuralFeature )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->getFeatureID(), -1 );
    BOOST_CHECK( eAttribute->getDefaultValue().empty() );
}

BOOST_AUTO_TEST_CASE( Accessors_EAttribute )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->isID(), false );
}

BOOST_AUTO_TEST_SUITE_END()
