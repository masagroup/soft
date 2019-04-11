// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_WEAKPTR_HPP_
#define ECORE_WEAKPTR_HPP_

#include <memory>


namespace ecore::impl
{
    template <typename T>
    bool is_uninitialized( std::weak_ptr<T> const& weak )
    {
        using wt = std::weak_ptr<T>;
        return !weak.owner_before( wt{} ) && !wt{}.owner_before( weak );
    }
}

#endif