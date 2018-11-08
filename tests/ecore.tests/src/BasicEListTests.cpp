#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/impl/BasicEList.hpp"
#include "ecore/impl/ImmutableEList.hpp"
#include <Windows.h>

using namespace ecore;
using namespace ecore::impl;

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

namespace std
{
    template <typename T>
    ostream& operator <<( ostream& os, const EList<T>& v )
    {
        bool first = true;
        os << "[";
        for (auto b : v)
        {
            os << (first ? "" : ",") << b;
            first = false;
        }
        os << "]";
        return os;
    }

    template <typename T>
    ostream& operator <<( ostream& os, const std::vector<T>& v )
    {
        bool first = true;
        os << "[";
        for (auto b : v)
        {
            os << (first ? "" : ",") << b;
            first = false;
        }
        os << "]";
        return os;
    }

    template <typename T>
    bool operator ==( const EList<T>& lhs, const EList<T>& rhs )
    {
        if (lhs.size() != rhs.size())
            return false;
        for (int i = 0; i < lhs.size(); ++i)
        {
            if (lhs.get( i ) != rhs.get( i ))
                return false;
        }
        return true;
    }

    template <typename T>
    bool operator ==( const EList<T>& lhs, const std::vector<T>& rhs )
    {
        if (lhs.size() != rhs.size())
            return false;
        for (int i = 0; i < lhs.size(); ++i)
        {
            if (lhs.get( i ) != rhs.at( i ))
                return false;
        }
        return true;
    }
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

    for (int i = 0; i < 10; ++i)
        list->add( i );

    int test = 0;
    for (int i : *list)
        BOOST_CHECK_EQUAL( i, test++ );
}

BOOST_AUTO_TEST_CASE( Iterators )
{
    std::shared_ptr<BasicEList<int>> list = std::make_shared<BasicEList<int>>();
    for (int i = 0; i < 10; ++i)
        list->add( i );

    int test = 0;
    for (int i : *list)
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

BOOST_AUTO_TEST_CASE( AddAll )
{
    {
        auto list = std::make_shared<BasicEList<int>>();
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 1, 2 } );
        BOOST_CHECK( list->addAll( other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 1, 2 } ) );
    }
    {
        auto list = std::make_shared<BasicEList<int>>( std::initializer_list<int>{ 1, 2 } );
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 3, 4 } );
        BOOST_CHECK( list->addAll( other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 1, 2 , 3 ,4 } ) );
    }
}

BOOST_AUTO_TEST_CASE( AddAll_Index )
{
    {
        auto list = std::make_shared<BasicEList<int>>( std::initializer_list<int>{ 1, 2 } );
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 3, 4 } );
        BOOST_CHECK( list->addAll( 0, other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 3,4,1,2 } ) );
    }
    {
        auto list = std::make_shared<BasicEList<int>>( std::initializer_list<int>{ 1, 2 } );
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 3, 4 } );
        BOOST_CHECK( list->addAll( 1, other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 1,3,4,2 } ) );
    }
    {
        auto list = std::make_shared<BasicEList<int>>( std::initializer_list<int>{ 1, 2 } );
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 3, 4 } );
        BOOST_CHECK( list->addAll( 2, other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 1,2,3,4 } ) );
    }
}

BOOST_AUTO_TEST_CASE( Unique_Add )
{
    auto list = std::make_shared<BasicEList<int, true>>();
    BOOST_CHECK( list->add( 1 ) );
    BOOST_CHECK( !list->add( 1 ) );
}

BOOST_AUTO_TEST_CASE( Unique_Add_Index )
{
    auto list = std::make_shared<BasicEList<int, true>>();
    list->add( 0, 1 );
    list->add( 0, 2 );
}

BOOST_AUTO_TEST_CASE( Unique_Add_Index_InvalidElement, *boost::unit_test::precondition( no_debugger() ) )
{
    auto list = std::make_shared<BasicEList<int, true>>();
    list->add( 0, 1 );
    BOOST_REQUIRE_THROW( list->add( 1, 1 ), boost::execution_exception );
}

BOOST_AUTO_TEST_CASE( Unique_AddAll )
{
    auto list = std::make_shared<BasicEList<int, true>>( std::initializer_list<int>{ 1, 2 } );
    auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 2, 3 } );
    BOOST_CHECK( list->addAll( other ) );
    BOOST_CHECK_EQUAL( *list, std::vector<int>( { 1,2,3 } ) );
}

BOOST_AUTO_TEST_CASE( Unique_AddAll_Index )
{
    {
        auto list = std::make_shared<BasicEList<int, true>>( std::initializer_list<int>{ 1, 2 } );
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 2, 3 } );
        BOOST_CHECK( list->addAll( 0, other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 3,1,2 } ) );
    }
    {
        auto list = std::make_shared<BasicEList<int, true>>( std::initializer_list<int>{ 1, 2 } );
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 2, 3 } );
        BOOST_CHECK( list->addAll( 1, other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 1,3,2 } ) );
    }
    {
        auto list = std::make_shared<BasicEList<int, true>>( std::initializer_list<int>{ 1, 2 } );
        auto other = std::make_shared<ImmutableEList<int>>( std::initializer_list<int>{ 2, 3 } );
        BOOST_CHECK( list->addAll( 2, other ) );
        BOOST_CHECK_EQUAL( *list, std::vector<int>( { 1,2,3 } ) );
    }
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
