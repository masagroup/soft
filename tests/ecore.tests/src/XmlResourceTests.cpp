#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EPackage.hpp"
#include "ecore/EClassifier.hpp"
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
    BOOST_CHECK_EQUAL( ePackage->getNsURI(), "http:///com.ibm.dynamic.example.bookStore.ecore");

    auto eClassifiers = ePackage->getEClassifiers();
    BOOST_CHECK_EQUAL( eClassifiers->size(), 1 );
    auto eClassifier = eClassifiers->get( 0 );
    BOOST_CHECK_EQUAL( eClassifier->getName(), "BookStore" );

}

BOOST_AUTO_TEST_SUITE_END()
