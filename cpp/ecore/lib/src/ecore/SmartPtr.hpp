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

    template <class T>
    std::shared_ptr<T> derived_pointer_cast( const std::shared_ptr<T>& s )
    {
        return s;
    }

    template <class T, class U>
    std::shared_ptr<U> derived_pointer_cast( const std::shared_ptr<T>& s )
    {
        if constexpr( is_virtual_base_of<T, U>::value )
            return std::dynamic_pointer_cast<U>( s );
        else
            return std::static_pointer_cast<U>( s );
    }

} // namespace ecore

#endif
