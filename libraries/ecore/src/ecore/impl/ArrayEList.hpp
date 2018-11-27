// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ARRAYELIST_HPP_
#define ECORE_ARRAYELIST_HPP_

#include "ecore/impl/AbstractArrayEList.hpp"

#include <vector>

namespace ecore::impl
{

    template <typename T, bool unique = false >
    class ArrayEList : public AbstractArrayEList<EList<T>,T, unique >
    {
    public:
        ArrayEList()
            : AbstractArrayEList<EList<T>, T, unique >()
        {
        }

        ArrayEList( const std::initializer_list<T>& init )
            : AbstractArrayEList<EList<T>, T, unique >( init )
        {
        }


        ArrayEList( const ArrayEList<T>& o )
            : AbstractArrayEList<EList<T>, T, unique >( o )
        {
        }

        virtual ~ArrayEList()
        {
        }
    };

}

#endif /* ECORE_ARRAYELIST_HPP_ */
