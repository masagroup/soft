#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/BasicEList.hpp"
#include <Windows.h>

using namespace ecore;

namespace
{
    struct no_debugger
    {
        boost::test_tools::assertion_result operator()( boost::unit_test::test_unit_id )
        {
            return !IsDebuggerPresent();
        }
    };
}

BOOST_AUTO_TEST_SUITE( BasicEListTests )

BOOST_AUTO_TEST_CASE( Constructor_Default )
{
    BOOST_CHECK_NO_THROW( BasicEList<int> list; );
}

BOOST_AUTO_TEST_CASE( Constructor_Initializer )
{
    BasicEList<int> list = { 1 , 2 };
}

BOOST_AUTO_TEST_CASE( Empty )
{
    {
        BasicEList<int> list;
        BOOST_CHECK( list.empty() );
    }
    {
        BasicEList<int> list = { 1 };
        BOOST_CHECK( !list.empty() );
    }
}


BOOST_AUTO_TEST_CASE( Size )
{
    {
        BasicEList<int> list;
        BOOST_CHECK_EQUAL( list.size(), 0 );
    }
    {
        BasicEList<int> list = { 1 , 2 };
        BOOST_CHECK_EQUAL( list.size(), 2 );
    }
}

BOOST_AUTO_TEST_CASE( Get )
{
    BasicEList<int> list = { 1 , 2 };
    BOOST_CHECK_EQUAL( list.get( 0 ), 1 );
    BOOST_CHECK_EQUAL( list.get( 1 ), 2 );
}

BOOST_AUTO_TEST_CASE( Iterators_Empty )
{
    std::shared_ptr<BasicEList<int>> list = std::make_shared<BasicEList<int>>();
    BOOST_CHECK( list->begin() == list->end() );

    for( int i = 0; i < 10; ++i )
        list->add( i );

    int test = 0;
    for( int i : *list )
        BOOST_CHECK_EQUAL( i, test++ );
}

BOOST_AUTO_TEST_CASE( Iterators )
{
    std::shared_ptr<BasicEList<int>> list = std::make_shared<BasicEList<int>>();
    for( int i = 0; i < 10; ++i )
        list->add( i );

    int test = 0;
    for( int i : *list )
        BOOST_CHECK_EQUAL( i, test++ );
}

BOOST_AUTO_TEST_CASE( IndexOf )
{
    std::shared_ptr<BasicEList<int>> list = std::make_shared<BasicEList<int>>();
    list->add( 1 );
    list->add( 2 );
    BOOST_CHECK_EQUAL( list->indexOf( 1 ), 0 );
    BOOST_CHECK_EQUAL( list->indexOf( 2 ), 1 );
    BOOST_CHECK_EQUAL( list->indexOf( 3 ), -1 );
}

BOOST_AUTO_TEST_CASE( Add )
{
    BasicEList<int> list;
    BOOST_CHECK_EQUAL( list.add( 1 ), true );
    BOOST_CHECK_EQUAL( list.add( 2 ), true );
    BOOST_CHECK_EQUAL( list.size(), 2 );
    BOOST_CHECK_EQUAL( list.get( 0 ), 1 );
    BOOST_CHECK_EQUAL( list.get( 1 ), 2 );
}

BOOST_AUTO_TEST_CASE( Add_InvalidIndex, *boost::unit_test::precondition( no_debugger() ) )
{
    BasicEList<int> list;
    BOOST_REQUIRE_THROW( list.add( 1, 0 ), boost::execution_exception );
}

BOOST_AUTO_TEST_CASE( Add_Index )
{
    BasicEList<int> list;
    list.add( 0, 1 );
    list.add( 0, 2 );
    BOOST_CHECK_EQUAL( list.size(), 2 );
    BOOST_CHECK_EQUAL( list.get( 0 ), 2 );
    BOOST_CHECK_EQUAL( list.get( 1 ), 1 );
}

BOOST_AUTO_TEST_CASE( Remove_InvalidIndex, *boost::unit_test::precondition( no_debugger() ) )
{
    BasicEList<int> list;
    BOOST_REQUIRE_THROW( list.remove( 0 ), boost::execution_exception );
}


BOOST_AUTO_TEST_CASE( Remove_Index )
{
    BasicEList<int> list = { 1 , 2 };
    int old = list.remove( 0 );
    BOOST_CHECK_EQUAL( old, 1 );
    BOOST_CHECK_EQUAL( list.size(), 1 );
}

BOOST_AUTO_TEST_CASE( Set_InvalidIndex, *boost::unit_test::precondition( no_debugger() ) )
{
    BasicEList<int> list = { 1 , 2 };
    BOOST_REQUIRE_THROW( list.set( 2, 3 ), boost::execution_exception );
}

BOOST_AUTO_TEST_CASE( Set )
{
    BasicEList<int> list = { 1 , 2 };
    list.set( 0, 3 );
    BOOST_CHECK_EQUAL( list.get( 0 ), 3 );
}

BOOST_AUTO_TEST_CASE( Clear )
{
    BasicEList<int> list = { 1 , 2 };
    list.clear();
    BOOST_CHECK( list.empty() );
}

BOOST_AUTO_TEST_SUITE_END()
