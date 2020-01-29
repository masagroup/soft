#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EList.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/tests/MockClassifier.hpp"
#include "ecore/tests/MockFactory.hpp"
#include "ecore/tests/MockObjectInternal.hpp"

using namespace ecore;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( EPackageTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    BOOST_CHECK( ePackage );
}

BOOST_AUTO_TEST_CASE( Accessors_Attributes )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    ePackage->setName( "ePackageName" );
    BOOST_CHECK_EQUAL( ePackage->getName(), "ePackageName" );

    ePackage->setNsPrefix( "eNsPrefix" );
    BOOST_CHECK_EQUAL( ePackage->getNsPrefix(), "eNsPrefix" );

    ePackage->setNsURI( "eNsURI" );
    BOOST_CHECK_EQUAL( ePackage->getNsURI(), "eNsURI" );
}

BOOST_AUTO_TEST_CASE( Accessors_FactoryInstance )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();
    auto mockFactory = std::make_shared<MockFactory>();
    auto mockInternal = std::make_shared<MockObjectInternal>();
    MOCK_EXPECT( mockFactory->getInternalNonConst ).returns( *mockInternal );
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( ePackage, EcorePackage::EFACTORY__EPACKAGE, nullptr ).returns( nullptr );
    ePackage->setEFactoryInstance( mockFactory );
    BOOST_CHECK_EQUAL( ePackage->getEFactoryInstance(), mockFactory );
}

BOOST_AUTO_TEST_CASE( Accessors_Classifiers )
{
    auto ecoreFactory = EcoreFactory::eInstance();
    auto ecorePackage = EcorePackage::eInstance();

    auto ePackage = ecoreFactory->createEPackage();

    // add classifiers in the package
    auto eClassifier1 = std::make_shared<MockClassifier>();
    auto eClassifier2 = std::make_shared<MockClassifier>();
    auto eClassifier3 = std::make_shared<MockClassifier>();
    auto mockInternal = std::make_shared<MockObjectInternal>();

    MOCK_EXPECT( eClassifier1->getName ).returns( "eClassifier1" );
    MOCK_EXPECT( eClassifier1->getInternalNonConst ).returns( *mockInternal );
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( ePackage, EcorePackage::ECLASSIFIER__EPACKAGE, nullptr ).returns( nullptr );
    MOCK_EXPECT( eClassifier2->getName ).returns( "eClassifier2" );
    MOCK_EXPECT( eClassifier2->getInternalNonConst ).returns( *mockInternal );
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( ePackage, EcorePackage::ECLASSIFIER__EPACKAGE, nullptr ).returns( nullptr );
    MOCK_EXPECT( eClassifier3->getName ).returns( "eClassifier3" );
    MOCK_EXPECT( eClassifier3->getInternalNonConst ).returns( *mockInternal );
    MOCK_EXPECT( mockInternal->eInverseAdd ).with( ePackage, EcorePackage::ECLASSIFIER__EPACKAGE, nullptr ).returns( nullptr );
    ePackage->getEClassifiers()->add( eClassifier1 );
    ePackage->getEClassifiers()->add( eClassifier2 );

    // retrieve them in the package
    BOOST_CHECK_EQUAL( ePackage->getEClassifier( "eClassifier1" ), eClassifier1 );
    BOOST_CHECK_EQUAL( ePackage->getEClassifier( "eClassifier2" ), eClassifier2 );

    // ensure that even if we add it after getting previous ones , the cache inside
    // package is still valid
    ePackage->getEClassifiers()->add( eClassifier3 );
    BOOST_CHECK_EQUAL( ePackage->getEClassifier( "eClassifier3" ), eClassifier3 );
}

BOOST_AUTO_TEST_SUITE_END()