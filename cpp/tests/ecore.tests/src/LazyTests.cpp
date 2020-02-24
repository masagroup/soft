#include <boost/test/unit_test.hpp>

#include "ecore/impl/Lazy.hpp"

using namespace ecore;
using namespace ecore::impl;

BOOST_AUTO_TEST_SUITE( LazyTests )


BOOST_AUTO_TEST_CASE( LazySimple_Return )
{
    Lazy<int> l( []()
    {
        return 1;
    }, 0 );
    BOOST_CHECK_EQUAL( l.get(), 1 );
    l = 2;
    BOOST_CHECK_EQUAL( l.get(), 2 );
}

BOOST_AUTO_TEST_CASE( LazySimple_NoReturn )
{
    bool called = false;
    Lazy<int,void> l( [&]()
    {
        called = true;
    }, 0 );
    BOOST_CHECK_EQUAL( l.get(), 0 );
    BOOST_CHECK( called );
}


BOOST_AUTO_TEST_CASE( LazyShared_Return )
{
    Lazy<std::shared_ptr<int>> l( []()
    {
        return std::make_shared<int>(1);
    });
    const auto& s = l.get();
    BOOST_CHECK( s );
    BOOST_CHECK_EQUAL( *s , 1 );
    l = std::make_shared<int>( 2 );
    BOOST_CHECK_EQUAL( *s, 2 );
    l = std::move( std::make_shared<int>( 3 ) );
    BOOST_CHECK_EQUAL( *s, 3 );
}

BOOST_AUTO_TEST_CASE( LazyShared_NoReturn )
{
    bool called = false;
    Lazy<std::shared_ptr<int>,void> l( [&]()
    {
        called = true;
    } );
    const auto& s = l.get();
    BOOST_CHECK( !s );
    BOOST_CHECK( called );
}

BOOST_AUTO_TEST_CASE( LazySharedClass_Return )
{
    class A
    {
    public:
        A()
            : value_( std::bind( &A::initValue , this ) )
        {
        }

        int getValue() const
        {
            return value_.get();
        }

    private:
        int initValue()
        {
            return 1;
        }

    private:
        Lazy<int> value_;
    };

    A a;
    BOOST_CHECK_EQUAL( a.getValue() , 1 );
}

BOOST_AUTO_TEST_CASE( LazyShared_Operators )
{
    Lazy<std::shared_ptr<int>> a( []
    {
        return std::make_shared<int>(1);
    });
    BOOST_CHECK( a == nullptr );
    BOOST_CHECK_EQUAL( *a.get(), 1 );
    BOOST_CHECK( a != nullptr );
}

BOOST_AUTO_TEST_CASE( LazyUnique_Return )
{
    Lazy<std::unique_ptr<int>> l( []()
    {
        return std::make_unique<int>( 1 );
    } );
    const auto& s = l.get();
    BOOST_CHECK( s );
    BOOST_CHECK_EQUAL( *s, 1 );
    l = std::make_unique<int>( 2 );
    BOOST_CHECK_EQUAL( *s, 2 );
    l = std::move( std::make_unique<int>( 3 ) );
    BOOST_CHECK_EQUAL( *s, 3 );
}

BOOST_AUTO_TEST_CASE( LazyUnique_NoReturn )
{
    bool called = false;
    Lazy<std::unique_ptr<int>,void> l( [&]()
    {
        called = true;
    } );
    const auto& s = l.get();
    BOOST_CHECK( !s );
    BOOST_CHECK( called );
}



BOOST_AUTO_TEST_SUITE_END()
