#include <boost/test/unit_test.hpp>

#include "ecore\TypeTraits.hpp"

using namespace ecore;

namespace
{
    struct A
    {
    };

    struct B : public A
    {
    };

    struct C : public virtual A
    {
    };

} // namespace

BOOST_AUTO_TEST_SUITE( TypeTraitsTests )

BOOST_AUTO_TEST_CASE( IsSharedPtr )
{
    BOOST_CHECK( is_shared_ptr<std::shared_ptr<A>>::value );
    BOOST_CHECK( !is_shared_ptr<A*>::value );
}

BOOST_AUTO_TEST_CASE( IsUnitialized )
{
    std::weak_ptr<A> w;
    BOOST_CHECK( is_uninitialized( w ) );
    
    std::shared_ptr<B> b = std::make_shared<B>();
    std::weak_ptr<B> bw = b;
    BOOST_CHECK( !is_uninitialized( bw ) );
}

BOOST_AUTO_TEST_CASE( IsVirtualBase )
{
    BOOST_CHECK( static_cast<bool>(is_virtual_base_of<A, C>::value) );
    BOOST_CHECK( !static_cast<bool>(is_virtual_base_of<A, B>::value) );
    BOOST_CHECK( !static_cast<bool>( is_virtual_base_of<A, A>::value ) );
}


BOOST_AUTO_TEST_SUITE_END()
