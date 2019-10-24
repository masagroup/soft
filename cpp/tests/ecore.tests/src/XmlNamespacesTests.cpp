#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/impl/XmlNamespaces.hpp"

using namespace ecore;
using namespace ecore::impl;

BOOST_AUTO_TEST_SUITE( XmlNamespacesTests )

BOOST_AUTO_TEST_CASE( Empty )
{
    XmlNamespaces namespaces;
    BOOST_CHECK( namespaces.getURI( "prefix" ).empty() );
    BOOST_CHECK( namespaces.getPrefix( "uri" ).empty() );
}

BOOST_AUTO_TEST_CASE( Context )
{
    XmlNamespaces namespaces;
    namespaces.pushContext();
    BOOST_CHECK( !namespaces.declarePrefix( "prefix", "uri" ) );
    BOOST_CHECK( namespaces.getURI( "prefix" ) == "uri" );
    BOOST_CHECK( namespaces.getPrefix( "uri" ) == "prefix" );

    namespaces.pushContext();
    BOOST_CHECK( !namespaces.declarePrefix( "prefix", "uri2" ) );
    BOOST_CHECK( namespaces.getURI( "prefix" ) == "uri2" );
    BOOST_CHECK( namespaces.getPrefix( "uri2" ) == "prefix" );

    namespaces.popContext();
    BOOST_CHECK( namespaces.getURI( "prefix" ) == "uri" );
    BOOST_CHECK( namespaces.getPrefix( "uri" ) == "prefix" );

    namespaces.popContext();
    BOOST_CHECK( namespaces.getURI( "prefix" ).empty() );
    BOOST_CHECK( namespaces.getPrefix( "uri" ).empty() );
}

BOOST_AUTO_TEST_CASE( Context_Remap )
{
    XmlNamespaces namespaces;
    namespaces.pushContext();
    BOOST_CHECK( !namespaces.declarePrefix( "prefix", "uri" ) );
    BOOST_CHECK( namespaces.getURI( "prefix" ) == "uri" );
    BOOST_CHECK( namespaces.getPrefix( "uri" ) == "prefix" );

    BOOST_CHECK( namespaces.declarePrefix( "prefix", "uri2" ) );
    BOOST_CHECK( namespaces.getURI( "prefix" ) == "uri2" );
    BOOST_CHECK( namespaces.getPrefix( "uri2" ) == "prefix" );
}

BOOST_AUTO_TEST_CASE( Context_NoRemap )
{
    XmlNamespaces namespaces;
    namespaces.pushContext();
    BOOST_CHECK( !namespaces.declarePrefix( "prefix", "uri" ) );
    BOOST_CHECK( namespaces.getURI( "prefix" ) == "uri" );
    BOOST_CHECK( namespaces.getPrefix( "uri" ) == "prefix" );

    namespaces.pushContext();
    BOOST_CHECK( !namespaces.declarePrefix( "prefix", "uri2" ) );
    BOOST_CHECK( namespaces.getURI( "prefix" ) == "uri2" );
    BOOST_CHECK( namespaces.getPrefix( "uri2" ) == "prefix" );
}


BOOST_AUTO_TEST_SUITE_END()
