#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EAttribute.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EList.hpp"
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
            eAttribute->eAdapters().add( eAdapter.get() );
            eAttribute->eSetDeliver( true );
        }

        ~AttributeNotificationsFixture()
        {
            eAttribute->eAdapters().remove( eAdapter.get() );
        }

    protected:
        std::shared_ptr<EAttribute> eAttribute;
        std::unique_ptr<MockAdapter> eAdapter;
    };
}

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
    BOOST_CHECK_EQUAL( eAttribute->eDeliver(), false );
    eAttribute->eSetDeliver( true );
    BOOST_CHECK_EQUAL( eAttribute->eDeliver(), true );
}


BOOST_AUTO_TEST_CASE( Accessors_ENamedElement )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->getName(), "" );
    eAttribute->setName( "toto" );
    BOOST_CHECK_EQUAL( eAttribute->getName(), "toto" );
}

BOOST_FIXTURE_TEST_CASE( Accessors_ENamedElement_Notifications , AttributeNotificationsFixture  )
{
    MOCK_EXPECT( eAdapter->notifyChanged ).with( [ = ]( const std::shared_ptr<ENotification>& n )
    {
        return n->getNotifier() == eAttribute
            && n->getFeature() == EcorePackage::eInstance()->getENamedElement_Name()
            && boost::any_cast<std::string>(n->getOldValue()) == ""
            && boost::any_cast<std::string>( n->getNewValue() ) == "toto"
            && n->getPosition() == -1;
    } ).once();

    eAttribute->setName( "toto" );
    BOOST_CHECK_EQUAL( eAttribute->getName(), "toto" );
}

BOOST_AUTO_TEST_CASE( Accessors_ETypedElement )
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

BOOST_AUTO_TEST_CASE( Accessors_EStructuralFeature )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->getFeatureID(), -1 );
    //BOOST_CHECK( eAttribute->getDefaultValue().empty() );
}

BOOST_AUTO_TEST_CASE( Accessors_EAttribute )
{
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK( eAttribute );
    BOOST_CHECK_EQUAL( eAttribute->isID(), false );
}



BOOST_AUTO_TEST_SUITE_END()
