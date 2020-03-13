// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_TYPETRAITS_HPP_
#define ECORE_TYPETRAITS_HPP_

#include <memory>
#include <type_traits>

namespace ecore
{
    template<typename T> struct is_shared_ptr : std::false_type
    {
    };

    template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
    {
    };

    template <typename T>
    bool is_uninitialized( std::weak_ptr<T> const& weak )
    {
        using wt = std::weak_ptr<T>;
        return !weak.owner_before( wt{} ) && !wt{}.owner_before( weak );
    }

    namespace detail
    {
        // Implementation based on the standard's rules of explicit type conversions.
        // A pointer to an object of *derived* class type may be explicitly converted to a pointer to an *unambiguous* *base* class type.
        // A pointer to an object of an *unambiguous* *non-virtual* *base* class type may be explicitly converted to a pointer of a
        // *derived* class type. Therefore Derived has a virtual base Base if and only if
        //   (1) a Derived* can be converted to Base* (so the base class is unambiguous, which comes necessarily from virtual inheritance)
        //   (2) a Base* cannot be converted to Derived* (so the base class is either ambiguous or virtual)
        // With both conditions true, Base must be a virtual base of Derived.
        // The "is_base_of" is only needed so the compiler can (but is not required to) error out if the types are incomplete.
        // This is in league with the the expected behaviour.

        template <class T, class U>
        constexpr bool is_virtual_base_impl( ... )
        {
            return true;
        }

        // C-style casts have the power to ignore inheritance visibility while still act as a static_cast.
        // They can also fall back to the behaviour of reinterpret_cast, which allows is_virtual_base_of to work on non-class types too.
        // Note that because we are casting pointers there can be no user-defined operators to interfere.
        template <class T, class U, typename std::void_t<decltype( (U*)( std::declval<T*>() ) )>* = nullptr>
        constexpr bool is_virtual_base_impl( int )
        {
            return false;
        }

    } // namespace detail

    template <class T, class U>
    struct is_virtual_base_of : public std::integral_constant<bool,
                                                              std::is_base_of<T, U>::value && detail::is_virtual_base_impl<T, U>( 0 )
                                                                  && !detail::is_virtual_base_impl<U, T>( 0 )>
    {
    };

}

#endif
