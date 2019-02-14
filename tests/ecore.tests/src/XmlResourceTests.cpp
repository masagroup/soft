#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/Stream.hpp"
#include "ecore/impl/XmlResource.hpp"

using namespace ecore;
using namespace ecore::impl;

BOOST_AUTO_TEST_SUITE( XmlResourceTests )

BOOST_AUTO_TEST_CASE( Load )
{
    XmlResource resource( Uri( "data/simple.book.ecore" ) );
    resource.load();

    auto contents = resource.getContents();
    BOOST_CHECK_EQUAL( contents->size(), 1 );

    auto ePackage = std::dynamic_pointer_cast<EPackage>( contents->get( 0 ) );
    BOOST_CHECK( ePackage );
    BOOST_CHECK_EQUAL( ePackage->getName(), "BookStorePackage" );
    BOOST_CHECK_EQUAL( ePackage->getNsPrefix(), "bookStore" );
    BOOST_CHECK_EQUAL( ePackage->getNsURI(), "http:///com.ibm.dynamic.example.bookStore.ecore" );

    auto eClassifiers = ePackage->getEClassifiers();
    BOOST_CHECK_EQUAL( eClassifiers->size(), 2 );

    auto eBookStore = eClassifiers->get( 0 );
    BOOST_CHECK_EQUAL( eBookStore->getName(), "BookStore" );
    auto eBookStoreClass = std::dynamic_pointer_cast<EClass>( eBookStore );
    BOOST_CHECK( eBookStoreClass );
    BOOST_CHECK_EQUAL( eBookStoreClass->getFeatureCount(), 3 );
    
    auto eOwnerFeature = eBookStoreClass->getEStructuralFeature( 0 );
    BOOST_CHECK_EQUAL( eOwnerFeature->getName(), "owner" );
    auto eOwnerAttribute = std::dynamic_pointer_cast<EAttribute>( eOwnerFeature );
    BOOST_CHECK( eOwnerAttribute );
    
    auto eLocationFeature = eBookStoreClass->getEStructuralFeature( 1 );
    BOOST_CHECK_EQUAL( eLocationFeature->getName(), "location" );
    auto eLocationAttribute = std::dynamic_pointer_cast<EAttribute>( eLocationFeature );
    BOOST_CHECK( eLocationAttribute );
    

    auto eBooksFeature = eBookStoreClass->getEStructuralFeature( 2 );
    BOOST_CHECK_EQUAL( eBooksFeature->getName(), "books" );
    auto eBooksReference = std::dynamic_pointer_cast<EReference>( eBooksFeature );
    BOOST_CHECK( eBooksReference );
    

    auto eBook = eClassifiers->get( 1 );
    BOOST_CHECK_EQUAL( eBook->getName(), "Book" );
}

BOOST_AUTO_TEST_SUITE_END()
