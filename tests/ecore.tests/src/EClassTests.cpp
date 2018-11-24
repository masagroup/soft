#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EClass.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EList.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"


using namespace ecore;


BOOST_AUTO_TEST_SUITE( EClassTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    BOOST_CHECK( eClass );
}

BOOST_AUTO_TEST_CASE( StructuralFeature_Add )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    BOOST_CHECK( eClass );
    auto eAttribute1 = EcoreFactory::eInstance()->createEAttribute();
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    auto eAttribute2 = EcoreFactory::eInstance()->createEAttribute();
    eClass->getEStructuralFeatures()->add( eAttribute2 );
    auto eReference1 = EcoreFactory::eInstance()->createEReference();
    eClass->getEStructuralFeatures()->add( 0 , eReference1 );
    auto eReference2 = EcoreFactory::eInstance()->createEReference();
    eClass->getEStructuralFeatures()->add( 0, eReference2 );
    BOOST_CHECK_EQUAL( eAttribute1->getFeatureID(), -1 );
    BOOST_CHECK_EQUAL( eAttribute2->getFeatureID(), -1 );
    BOOST_CHECK_EQUAL( eReference1->getFeatureID(), -1 );
    BOOST_CHECK_EQUAL( eReference2->getFeatureID(), -1 );

}


BOOST_AUTO_TEST_CASE( StructuralFeature_Get )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    BOOST_CHECK( eClass );
    
}


BOOST_AUTO_TEST_SUITE_END()
