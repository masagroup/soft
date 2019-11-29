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
    XmlResource resource( URI( "data/bookStore.ecore" ) );
    resource.load();

    BOOST_CHECK( resource.isLoaded() );
    BOOST_CHECK( resource.getWarnings()->empty() );
    BOOST_CHECK( resource.getErrors()->empty() );

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
    auto eLocationType = eLocationAttribute->getEAttributeType();

    auto eBooksFeature = eBookStoreClass->getEStructuralFeature( 2 );
    BOOST_CHECK_EQUAL( eBooksFeature->getName(), "books" );
    auto eBooksReference = std::dynamic_pointer_cast<EReference>( eBooksFeature );
    BOOST_CHECK( eBooksReference );
    
    auto eBook = eClassifiers->get( 1 );
    BOOST_CHECK_EQUAL( eBook->getName(), "Book" );
    auto eBookClass = std::dynamic_pointer_cast<EClass>( eBook );
    BOOST_CHECK( eBookClass );
    BOOST_CHECK_EQUAL( eBookClass->getFeatureCount(), 2 );
    
    auto eNameFeature = eBookClass->getEStructuralFeature( 0 );
    BOOST_CHECK_EQUAL( eNameFeature->getName(), "name" );
    auto eNameAttribute = std::dynamic_pointer_cast<EAttribute>( eNameFeature );
    BOOST_CHECK( eNameAttribute );
   
    auto eISBNFeature = eBookClass->getEStructuralFeature( 1 );
    BOOST_CHECK_EQUAL( eISBNFeature->getName(), "isbn" );
    auto eISBNAttribute = std::dynamic_pointer_cast<EAttribute>( eISBNFeature );
    BOOST_CHECK( eISBNAttribute );

    // check resolved reference
    BOOST_CHECK_EQUAL(eBooksReference->getEReferenceType(), eBookClass);
}

BOOST_AUTO_TEST_SUITE_END()
