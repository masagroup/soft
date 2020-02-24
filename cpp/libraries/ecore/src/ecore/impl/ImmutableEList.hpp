// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMMUTABLE_ELIST_HPP_
#define ECORE_IMMUTABLE_ELIST_HPP_

#include "ecore/impl/AbstractImmutableEList.hpp"

#include <vector>

namespace ecore::impl
{

    template <typename T>
    class ImmutableEList : public AbstractImmutableEList<T>
    {
    public:
        ImmutableEList() = default;

        ImmutableEList( const std::vector<T>&& v )
            : v_( v )
        {
        }

        ImmutableEList( std::initializer_list<T> l )
            : v_( l )
        {
        }

        virtual ~ImmutableEList()
        {
        }
        
        virtual T get( std::size_t pos ) const
        {
            return v_.at( pos );
        }

        virtual std::size_t size() const
        {
            return v_.size();
        }

        virtual bool empty() const
        {
            return v_.empty();
        }

    private:
        std::vector<T> v_;
    };
} // namespace ecore::impl

#endif /* ECORE_IMMUTABLE_ELIST_HPP_ */
