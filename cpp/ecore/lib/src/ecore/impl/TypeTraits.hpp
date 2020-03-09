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

namespace ecore::impl
{
    template<typename T> struct is_shared_ptr : std::false_type
    {
    };

    template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
    {
    };
}

#endif