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

#include "ecore/EList.hpp"

#include <vector>

namespace ecore {

    template <typename T>
    class ImmutableEList : public EList<T> {
    public:
        ImmutableEList( const std::vector<T>&& v )
            : v_( v )
        {
        }

        virtual ~ImmutableEList() {}

        virtual bool add( const T& e ) { throw "UnsupportedOperationException";  }

        virtual bool addAll( const std::shared_ptr<EList<T>>& l ) { throw "UnsupportedOperationException"; }

        virtual void add( std::size_t pos, const T& e ) { throw "UnsupportedOperationException"; }

        virtual bool addAll( std::size_t pos, const std::shared_ptr<EList<T>>& l ) { throw "UnsupportedOperationException"; }

        virtual T get( std::size_t pos ) const { return v_.at( pos );  }

        virtual void set( std::size_t pos, const T& e ) { throw "UnsupportedOperationException"; }

        virtual T remove( std::size_t pos ) { throw "UnsupportedOperationException"; }

        virtual bool remove( const T& e ) { throw "UnsupportedOperationException"; }

        virtual std::size_t size() const { return v_.size(); }

        virtual void clear() { throw "UnsupportedOperationException"; }

        virtual bool empty() const { return v_.empty(); }

    private:
        std::vector<T> v_;
    };
}

#endif /* ECORE_IMMUTABLE_ELIST_HPP_ */
