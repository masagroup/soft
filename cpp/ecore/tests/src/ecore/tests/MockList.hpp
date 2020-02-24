// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_LIST_HPP_
#define ECORE_MOCK_LIST_HPP_

#include "ecore/EList.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    template <typename T>
    MOCK_BASE_CLASS( MockList, EList<T> )
    {
        MOCK_METHOD_TPL( add, 1, bool( const T& ), add );
        MOCK_METHOD_TPL( add, 2, void( std::size_t, const T& ), addIndex );
        MOCK_METHOD_TPL( addAll, 1, bool( const EList<T>& ), addAll );
        MOCK_METHOD_TPL( addAll, 2, bool( std::size_t, const EList<T>& ), addAllIndex );
        MOCK_METHOD_TPL( move, 2, void( std::size_t, const T& ), move );
        MOCK_METHOD_TPL( move, 2, T( std::size_t, std::size_t ), moveIndex );
        MOCK_CONST_METHOD_TPL( get, 1, T( std::size_t ));
        MOCK_METHOD_TPL( set, 2, void( std::size_t, const T& ));
        MOCK_METHOD_TPL( remove, 1, T( std::size_t ), removeIndex );
        MOCK_METHOD_TPL( remove, 1, bool( const T& ), removeObject );
        MOCK_CONST_METHOD_TPL( size, 0, std::size_t());
        MOCK_METHOD_TPL( clear, 0, void());
        MOCK_CONST_METHOD_TPL( empty, 0, bool());
    };
}

#endif
