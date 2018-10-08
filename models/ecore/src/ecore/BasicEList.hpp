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
        typedef typename std::shared_ptr<T> T_Element;

        virtual ~BasicEList() {}

        virtual void addUnique(const T_Element& e)
        {
            std::size_t pos = size();
            v_.push_back(e);
            didAdd( pos, e);
            didChange();
        }

        virtual void addUnique( std::size_t pos,  const T_Element& e) {
            v_.insert( v_.begin() + pos , e );
            didAdd(pos, e);
            didChange();
        }

        virtual void setUnique(std::size_t pos, const T_Element& e) {
            v_[ pos ] = e;
        }

        virtual const T_Element& get( std::size_t pos ) const {
            return v_.at( pos );
        }

        virtual T_Element get( std::size_t pos ) {
            return v_.at( pos );
        }

        virtual T_Element primitiveGet( std::size_t pos ) const
        {
            return v_[ pos ];
        }

        virtual T_Element remove( std::size_t pos )
        {
            auto it = v_.begin() + pos;
            T_Element oldObject = std::move( *it );
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
            std::vector<T_Element> oldObjects = v_;
            v_.clear();
            didClear( oldObjects);
        }

        virtual bool contains(const T_Element& value) const
        {
            return std::find(v_.begin(), v_.end(), value) != v_.end();
        }

        virtual std::size_t indexOf(const T_Element& value, std::size_t from = 0) const {
            return std::distance(v_.begin(), std::find(v_.begin(), v_.end(), value));
        }

        virtual iterator begin()
        {
            return v_.begin();
        }

        virtual const_iterator begin() const
        {
            return v_.begin();
        }

        virtual iterator end()
        {
            return v_.end();
        }

        virtual const_iterator end() const
        {
            return v_.end();
        }


    private:
        std::vector<T_Element> v_;
    };

}

#endif /* ECORE_ELIST_HPP_ */
