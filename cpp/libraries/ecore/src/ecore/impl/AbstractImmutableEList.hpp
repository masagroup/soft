// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACT_IMMUTABLE_ELIST_HPP_
#define ECORE_ABSTRACT_IMMUTABLE_ELIST_HPP_

#include "ecore/EList.hpp"

namespace ecore::impl
{

    template <typename T>
    class AbstractImmutableEList : public EList<T>
    {
    public:
        AbstractImmutableEList() = default;

        virtual ~AbstractImmutableEList() = default;
        
        virtual bool add( const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool addAll( const EList<T>& l )
        {
            throw "UnsupportedOperationException";
        }

        virtual void add( std::size_t pos, const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool addAll( std::size_t pos, const EList<T>& l )
        {
            throw "UnsupportedOperationException";
        }

        virtual void move( std::size_t oldPos, const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual T move( std::size_t oldPos, std::size_t newPos )
        {
            throw "UnsupportedOperationException";
        }

        virtual void set( std::size_t pos, const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual T remove( std::size_t pos )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool remove( const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual void clear()
        {
            throw "UnsupportedOperationException";
        }
    };
} // namespace ecore::impl

#endif /* ECORE_ABSTRACT_IMMUTABLE_ELIST_HPP_ */
