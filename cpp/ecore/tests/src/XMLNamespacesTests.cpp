#include <boost/test/unit_test.hpp>

#include "ecore/impl/XMLNamespaces.hpp"

using namespace ecore;
using namespace ecore::impl;

BOOST_AUTO_TEST_SUITE( XMLNamespacesTests )

BOOST_AUTO_TEST_CASE(NoContext)
{
    XMLNamespaces namespaces;
    BOOST_CHECK(namespaces.getURI("prefix").empty());
    BOOST_CHECK(namespaces.getPrefix("uri").empty());
}

BOOST_AUTO_TEST_CASE( Empty )
{
    XMLNamespaces namespaces;
    namespaces.pushContext();
    BOOST_CHECK( namespaces.getURI( "prefix" ).empty() );
    BOOST_CHECK( namespaces.getPrefix( "uri" ).empty() );
    auto c = namespaces.popContext();
    BOOST_CHECK(c.empty());
}

BOOST_AUTO_TEST_CASE( Context )
{
    XMLNamespaces namespaces;
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
    XMLNamespaces namespaces;
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
    XMLNamespaces namespaces;
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
