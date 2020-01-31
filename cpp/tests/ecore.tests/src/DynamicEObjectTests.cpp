#include <boost/test/unit_test.hpp>

#include "ecore/EcorePackage.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EList.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/impl/DynamicEObjectImpl.hpp"
#include "ecore/tests/MockClass.hpp"
#include "ecore/tests/MockList.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( DynamicEObjectTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto eObject = std::make_shared<DynamicEObjectImpl>();
    eObject->setThisPtr( eObject );
    BOOST_CHECK_EQUAL( eObject, eObject->getThisPtr() );
}

BOOST_AUTO_TEST_CASE( EClass_Mock )
{
    auto eObject = std::make_shared<DynamicEObjectImpl>();
    eObject->setThisPtr( eObject );
    BOOST_CHECK_EQUAL( EcorePackage::eInstance()->getEObject(), eObject->eClass() );

    auto mockAdapters = std::make_shared<MockList<EAdapter*>>();
    auto mockClass = std::make_shared<MockClass>();
    MOCK_EXPECT( mockClass->getFeatureCount ).returns( 0 );
    MOCK_EXPECT( mockClass->eAdapters ).returns( *mockAdapters );
    MOCK_EXPECT( mockAdapters->add ).with(mock::any).returns(true);

    eObject->setEClass( mockClass );
    BOOST_CHECK_EQUAL( mockClass, eObject->eClass() );
}

BOOST_AUTO_TEST_CASE( EClass )
{
    auto eObject = std::make_shared<DynamicEObjectImpl>();
    eObject->setThisPtr( eObject );
    
    auto eClass = EcoreFactory::eInstance()->createEClass();
    eObject->setEClass( eClass );
    BOOST_CHECK_EQUAL( eClass, eObject->eClass() );
}


BOOST_AUTO_TEST_CASE( Attribute )
{
    auto eObject = std::make_shared<DynamicEObjectImpl>();
    eObject->setThisPtr( eObject );

    auto eClass = EcoreFactory::eInstance()->createEClass();
    eObject->setEClass( eClass );
    BOOST_CHECK_EQUAL( eClass, eObject->eClass() );

    auto eFeature = EcoreFactory::eInstance()->createEAttribute();
    eClass->getEStructuralFeatures()->add( eFeature );

    BOOST_CHECK( eObject->eGet( eFeature ).empty() );
    eObject->eSet( eFeature, 1 );
    BOOST_CHECK_EQUAL( eObject->eGet( eFeature ), 1 );
}


BOOST_AUTO_TEST_SUITE_END()