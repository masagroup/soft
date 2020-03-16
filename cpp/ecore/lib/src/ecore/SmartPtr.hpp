// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_SMARTPTR_HPP_
#define ECORE_SMARTPTR_HPP_

#include "ecore/TypeTraits.hpp"
#include <memory>

namespace ecore
{
    
    template <class T, class U>
    std::weak_ptr<T> static_pointer_cast( std::weak_ptr<U> const& r )
    {
        return std::static_pointer_cast<T>( r.lock() );
    }

    template <class T, class U>
    std::shared_ptr<T> derived_pointer_cast( const std::shared_ptr<U>& s )
    {
        if constexpr( std::is_same<T, U>::value )
            return s;
        if constexpr( ecore::is_virtual_base_of<U, T>::value )
            return std::dynamic_pointer_cast<T>( s );
        else
            return std::static_pointer_cast<T>( s );
    }

} // namespace ecore

#endif
