#include <boost/test/unit_test.hpp>

#include "ecore\SmartPtr.hpp"

using namespace ecore;

namespace
{
    class A
    {
    public:
        virtual ~A() = default;
    };

    class B : public A
    {
    public:
        virtual ~B() = default;
    };

    class C : public virtual A
    {
    public:
        virtual ~C() = default;
    };

} // namespace

BOOST_AUTO_TEST_SUITE( SmartPtrTests )

BOOST_AUTO_TEST_CASE( DerivedPointerCast_Static )
{
    auto b = std::make_shared<B>();
    auto a = std::static_pointer_cast<A>( b );
    
    auto b1 = derived_pointer_cast<B>( b );
    BOOST_CHECK_EQUAL( b, b1 );

    auto b2 = derived_pointer_cast<B>( a );
    BOOST_CHECK_EQUAL( b, b2 );

    auto a2 = derived_pointer_cast<A>( b );
    BOOST_CHECK_EQUAL( a, a2 );
}

BOOST_AUTO_TEST_CASE( DerivedPointerCast_Dynamic )
{
    auto c = std::make_shared<C>();
    auto a = std::static_pointer_cast<A>( c );

    auto c1 = derived_pointer_cast<C>( c );
    BOOST_CHECK_EQUAL( c, c1 );

    auto c2 = derived_pointer_cast<C>( a );
    BOOST_CHECK_EQUAL( c, c2 );

    auto a2 = derived_pointer_cast<A>( c );
    BOOST_CHECK_EQUAL( a, a2 );
}



BOOST_AUTO_TEST_SUITE_END()
