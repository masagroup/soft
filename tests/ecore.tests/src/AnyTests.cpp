#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/Stream.hpp"
#include "ecore/Any.hpp"
#include "ecore/tests/MockObject.hpp"

using namespace ecore;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE( AnyTests )

BOOST_AUTO_TEST_CASE( Constructor_Default )
{
    Any a;
    BOOST_CHECK( a.empty() );
    BOOST_CHECK_EQUAL( &a.type(), &typeid( void ) );
}

BOOST_AUTO_TEST_CASE( Constructor_Value_Small )
{
    Any a( 1 );
    BOOST_CHECK( !a.empty() );
    BOOST_CHECK_EQUAL( &a.type() , &typeid(int) );
}

BOOST_AUTO_TEST_CASE( Constructor_Value_Big )
{
    Any a( std::make_shared<MockObject>() );
    BOOST_CHECK( !a.empty() );
    BOOST_CHECK_EQUAL( &a.type(), &typeid( std::shared_ptr<MockObject>));
}

BOOST_AUTO_TEST_CASE( Constructor_Value_Move )
{
    Any a( std::make_shared<MockObject>() );
    BOOST_CHECK( !a.empty() );
    BOOST_CHECK_EQUAL( &a.type(), &typeid( std::shared_ptr<MockObject> ) );
}

BOOST_AUTO_TEST_CASE( Constructor_Copy_Small )
{
    Any a( 1 );
    Any b( a );
    BOOST_CHECK( !b.empty() );
    BOOST_CHECK_EQUAL( &b.type(), &typeid( int ) );
}

BOOST_AUTO_TEST_CASE( Constructor_Copy_Big )
{
    Any a( std::make_shared<MockObject>() );
    Any b( a );
    BOOST_CHECK( !b.empty() );
    BOOST_CHECK_EQUAL( &b.type(), &typeid( std::shared_ptr<MockObject> ) );
}

BOOST_AUTO_TEST_CASE( Constructor_Move_Small )
{
    Any a( 1 );
    Any b( std::move( a ) );
    BOOST_CHECK( a.empty() );
    BOOST_CHECK( !b.empty() );
    BOOST_CHECK_EQUAL( &b.type(), &typeid( int ) );
}


BOOST_AUTO_TEST_CASE( Constructor_Move_Big )
{
    Any a( std::make_shared<MockObject>() );
    Any b( std::move( a ) );
    BOOST_CHECK( a.empty() );
    BOOST_CHECK( !b.empty() );
    BOOST_CHECK_EQUAL( &b.type(), &typeid( std::shared_ptr<MockObject> ) );
}


BOOST_AUTO_TEST_CASE( AnyCast )
{
    {
        Any a( 1 );
        BOOST_CHECK_EQUAL( anyCast<int>( a ), 1 );
    }
    {
        auto m = std::make_shared<MockObject>();
        Any a( m );
        BOOST_CHECK_EQUAL( anyCast<std::shared_ptr<MockObject>>( a ), m );
    }
    {
        std::string w = "test";
        Any a( w );
        BOOST_CHECK_EQUAL( anyCast<std::string>( a ), w );
    }
}

BOOST_AUTO_TEST_CASE( BadCast )
{
    {
        Any a;
        BOOST_CHECK_THROW( anyCast<long>( a ), BadAnyCast );
    }
    {
        Any a( 1 );
        BOOST_CHECK_THROW( anyCast<long>( a ), BadAnyCast );
    }
}

BOOST_AUTO_TEST_CASE( Reset )
{
    Any a( 1 );
    a.reset();
    BOOST_CHECK( a.empty() );
}

BOOST_AUTO_TEST_CASE( Swap )
{
    Any a( 1 );
    Any b( std::make_shared<MockObject>() );
    swap( a, b );
    BOOST_CHECK_EQUAL( &a.type(), &typeid( std::shared_ptr<MockObject> ) );
    BOOST_CHECK_EQUAL( &b.type(), &typeid( int ) );
    BOOST_CHECK_EQUAL( anyCast<int>( b ), 1 );
}

BOOST_AUTO_TEST_CASE( Affectation )
{
    {
        Any a( std::make_shared<MockObject>() );
        Any b( 1 );
        a = b;
    }
    {
        Any a( 1 );
        Any b( std::make_shared<MockObject>() );
        a = b;
    }
}

BOOST_AUTO_TEST_CASE( Affectation_Value )
{
    {
        Any a( std::make_shared<MockObject>() );
        a = std::vector < std::size_t>{ 1, 2 };
    }
    {
        Any a( std::make_shared<MockObject>() );
        a = 1;
    }
}

BOOST_AUTO_TEST_CASE( Comparison )
{
    {
        Any a;
        Any b;
        BOOST_CHECK( a == b );
    }
    {
        Any a( std::make_shared<MockObject>() );
        Any b( 1 );
        BOOST_CHECK( a != b );
    }
}

BOOST_AUTO_TEST_CASE( Serialization )
{
    {
        Any a(1);
        std::ostringstream s;
        s << a;
        BOOST_CHECK_EQUAL( s.str() , "1" );
    }
    {
        Any a;
        std::ostringstream s;
        s << a;
        BOOST_CHECK_EQUAL( s.str(), "" );
    }
    {
        Any a( std::vector<int>{ 1, 2} );
        std::ostringstream s;
        s << a;
        BOOST_CHECK_EQUAL( s.str(), "[1,2]" );
    }
}



BOOST_AUTO_TEST_SUITE_END()