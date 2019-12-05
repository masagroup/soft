#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/Stream.hpp"
#include "ecore/impl/ArrayEList.hpp"
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
} // namespace

namespace std
{
    template <typename T>
    ostream& operator<<( ostream& os, const std::vector<T>& v )
    {
        return print_container( os, v );
    }

    template <typename T>
    bool operator==( const EList<T>& lhs, const std::vector<T>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }
} // namespace std

BOOST_AUTO_TEST_SUITE( BasicEListTests )

BOOST_AUTO_TEST_CASE( Constructor_Default )
{
    BOOST_CHECK_NO_THROW( ArrayEList<int> list; );
}

BOOST_AUTO_TEST_CASE( Constructor_Initializer )
{
    ArrayEList<int> list = {1, 2};
    BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 2} ) );
}

BOOST_AUTO_TEST_CASE( Empty )
{
    {
        ArrayEList<int> list;
        BOOST_CHECK( list.empty() );
    }
    {
        ArrayEList<int> list = {1};
        BOOST_CHECK( !list.empty() );
    }
}

BOOST_AUTO_TEST_CASE( Size )
{
    {
        ArrayEList<int> list;
        BOOST_CHECK_EQUAL( list.size(), 0 );
    }
    {
        ArrayEList<int> list = {1, 2};
        BOOST_CHECK_EQUAL( list.size(), 2 );
    }
}

BOOST_AUTO_TEST_CASE( Get )
{
    ArrayEList<int> list = {1, 2};
    BOOST_CHECK_EQUAL( list.get( 0 ), 1 );
    BOOST_CHECK_EQUAL( list.get( 1 ), 2 );
}

BOOST_AUTO_TEST_CASE( Iterators_Empty )
{
    ArrayEList<int> list;
    BOOST_CHECK( list.begin() == list.end() );

    for( int i = 0; i < 10; ++i )
        list.add( i );

    int test = 0;
    for( int i : list )
        BOOST_CHECK_EQUAL( i, test++ );
}

BOOST_AUTO_TEST_CASE( Iterators )
{
    ArrayEList<int> list;
    for( int i = 0; i < 10; ++i )
        list.add( i );

    int test = 0;
    for( int i : list )
        BOOST_CHECK_EQUAL( i, test++ );
}

BOOST_AUTO_TEST_CASE( IndexOf )
{
    ArrayEList<int> list;
    list.add( 1 );
    list.add( 2 );
    BOOST_CHECK_EQUAL( list.indexOf( 1 ), 0 );
    BOOST_CHECK_EQUAL( list.indexOf( 2 ), 1 );
    BOOST_CHECK_EQUAL( list.indexOf( 3 ), -1 );
}

BOOST_AUTO_TEST_CASE( Add )
{
    ArrayEList<int> list;
    BOOST_CHECK_EQUAL( list.add( 1 ), true );
    BOOST_CHECK_EQUAL( list.add( 2 ), true );
    BOOST_CHECK_EQUAL( list.size(), 2 );
    BOOST_CHECK_EQUAL( list.get( 0 ), 1 );
    BOOST_CHECK_EQUAL( list.get( 1 ), 2 );
}

#ifdef _DEBUG
BOOST_AUTO_TEST_CASE( Add_InvalidIndex, *boost::unit_test::precondition( no_debugger() ) )
{
    ArrayEList<int> list;
    BOOST_REQUIRE_THROW( list.add( 1, 0 ), boost::execution_exception );
}
#endif

BOOST_AUTO_TEST_CASE( Add_Index )
{
    ArrayEList<int> list;
    list.add( 0, 1 );
    list.add( 0, 2 );
    BOOST_CHECK_EQUAL( list.size(), 2 );
    BOOST_CHECK_EQUAL( list.get( 0 ), 2 );
    BOOST_CHECK_EQUAL( list.get( 1 ), 1 );
}

BOOST_AUTO_TEST_CASE( AddAll )
{
    {
        ArrayEList<int> list;
        ImmutableEList<int> other( std::initializer_list<int>{1, 2} );
        BOOST_CHECK( list.addAll( other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 2} ) );
    }
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        BOOST_CHECK( list.addAll( other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 2, 3, 4} ) );
    }
}

BOOST_AUTO_TEST_CASE( AddAll_Index )
{
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        BOOST_CHECK( list.addAll( 0, other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {3, 4, 1, 2} ) );
    }
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        BOOST_CHECK( list.addAll( 1, other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 3, 4, 2} ) );
    }
    {
        ArrayEList<int> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{3, 4} );
        BOOST_CHECK( list.addAll( 2, other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 2, 3, 4} ) );
    }
}

BOOST_AUTO_TEST_CASE( Unique_Add )
{
    ArrayEList<int, true> list;
    BOOST_CHECK( list.add( 1 ) );
    BOOST_CHECK( !list.add( 1 ) );
}

BOOST_AUTO_TEST_CASE( Unique_Add_Index )
{
    ArrayEList<int, true> list;
    list.add( 0, 1 );
    list.add( 0, 2 );
    BOOST_CHECK_EQUAL( list, std::vector<int>( {2, 1} ) );
}

#ifdef _DEBUG
BOOST_AUTO_TEST_CASE( Unique_Add_Index_InvalidElement, *boost::unit_test::precondition( no_debugger() ) )
{
    ArrayEList<int, true> list;
    list.add( 0, 1 );
    BOOST_REQUIRE_THROW( list.add( 1, 1 ), boost::execution_exception );
}
#endif // DEBUG

BOOST_AUTO_TEST_CASE( Unique_AddAll )
{
    ArrayEList<int, true> list( std::initializer_list<int>{1, 2} );
    ImmutableEList<int> other( std::initializer_list<int>{2, 3} );
    BOOST_CHECK( list.addAll( other ) );
    BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 2, 3} ) );
}

BOOST_AUTO_TEST_CASE( Unique_AddAll_Index )
{
    {
        ArrayEList<int, true> list = {1, 2};
        ImmutableEList<int> other = {2, 3};
        BOOST_CHECK( list.addAll( 0, other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {3, 1, 2} ) );
    }
    {
        ArrayEList<int, true> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{2, 3} );
        BOOST_CHECK( list.addAll( 1, other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 3, 2} ) );
    }
    {
        ArrayEList<int, true> list( std::initializer_list<int>{1, 2} );
        ImmutableEList<int> other( std::initializer_list<int>{2, 3} );
        BOOST_CHECK( list.addAll( 2, other ) );
        BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 2, 3} ) );
    }
}

#ifdef _DEBUG
BOOST_AUTO_TEST_CASE( Remove_InvalidIndex, *boost::unit_test::precondition( no_debugger() ) )
{
    ArrayEList<int> list;
    BOOST_REQUIRE_THROW( list.remove( 0 ), boost::execution_exception );
}
#endif

BOOST_AUTO_TEST_CASE( Remove_Index )
{
    ArrayEList<int> list = {1, 2};
    int old = list.remove( 0 );
    BOOST_CHECK_EQUAL( old, 1 );
    BOOST_CHECK_EQUAL( list.size(), 1 );
}

#ifdef _DEBUG
BOOST_AUTO_TEST_CASE( Set_InvalidIndex, *boost::unit_test::precondition( no_debugger() ) )
{
    ArrayEList<int> list = {1, 2};
    BOOST_REQUIRE_THROW( list.set( 2, 3 ), boost::execution_exception );
}
#endif

BOOST_AUTO_TEST_CASE( Set )
{
    ArrayEList<int> list = {1, 2};
    list.set( 0, 3 );
    BOOST_CHECK_EQUAL( list.get( 0 ), 3 );
}

BOOST_AUTO_TEST_CASE( Clear )
{
    ArrayEList<int> list = {1, 2};
    list.clear();
    BOOST_CHECK( list.empty() );
}

BOOST_AUTO_TEST_CASE( Move )
{
    ArrayEList<int> list = {1, 2, 3, 4, 5};
    list.move( 3, 2 );
    BOOST_CHECK_EQUAL( list, std::vector<int>( {1, 2, 4, 3, 5} ) );
}

BOOST_AUTO_TEST_CASE( Delegate_SharedPtr )
{
    class A
    {
    public:
        A() = default;

        virtual ~A() = default;
    };

    class B : public A
    {
    public:
        B() = default;

        virtual ~B() = default;
    };

    auto init = std::initializer_list<std::shared_ptr<B>>( {std::make_shared<B>(), std::make_shared<B>()} );
    auto list = std::make_shared<ArrayEList<std::shared_ptr<B>>>( init );
    auto delegate = list->asEListOf<std::shared_ptr<A>>();
    BOOST_CHECK_EQUAL( delegate->size(), 2 );

    auto newB = std::make_shared<B>();
    BOOST_CHECK( delegate->add( newB ) );
    BOOST_CHECK_EQUAL( list->size(), 3 );
    BOOST_CHECK_EQUAL( list->get( 2 ), newB );
}

BOOST_AUTO_TEST_CASE( Delegate_Any )
{
    class A
    {
    public:
        A() = default;

        virtual ~A() = default;
    };

    auto init = std::initializer_list<std::shared_ptr<A>>( {std::make_shared<A>(), std::make_shared<A>()} );
    auto list = std::make_shared<ArrayEList<std::shared_ptr<A>>>( init );
    auto delegate = list->asEListOf<Any>();

    auto newA = std::make_shared<A>();
    BOOST_CHECK( delegate->add( newA) );
    BOOST_CHECK_EQUAL( list->size(), 3 );
    BOOST_CHECK_EQUAL( list->get( 2 ), newA );
    BOOST_CHECK_EQUAL( delegate->get( 2 ), newA );
}

BOOST_AUTO_TEST_SUITE_END()
