// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASICELIST_HPP_
#define ECORE_BASICELIST_HPP_

#include "ecore/AbstractEList.hpp"

#include <vector>


namespace ecore {

    template <typename T>
    class BasicEList : public AbstractEList<T> {
    public:
        virtual ~BasicEList() {}

        virtual void addUnique(const T& e)
        {
            std::size_t pos = size();
            v_.push_back(e);
            didAdd( pos, e);
            didChange();
        }

        virtual void addUnique( std::size_t pos,  const T& e) {
            v_.insert( v_.begin() + pos , e );
            didAdd(pos, e);
            didChange();
        }

        virtual void setUnique(std::size_t pos, const T& e) {
            v_[ pos ] = e;
        }

        virtual const T& get( std::size_t pos ) const {
            return v_.at( pos );
        }

        virtual T get( std::size_t pos ) {
            return v_.at( pos );
        }

        virtual T primitiveGet( std::size_t pos ) const
        {
            return v_[ pos ];
        }

        virtual T remove( std::size_t pos )
        {
            auto it = v_.begin() + pos;
            T oldObject = std::move( *it );
            v_.erase( it );
            didRemove(pos, oldObject);
            didChange();
            return oldObject;
        }


        virtual std::size_t size() const
        {
            return v_.size();
        }

        virtual void clear()
        {
            std::vector<T> oldObjects = v_;
            v_.clear();
            didClear( oldObjects);
        }

    private:
        std::vector<T> v_;
    };

}

#endif /* ECORE_ELIST_HPP_ */
