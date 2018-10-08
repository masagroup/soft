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
            int size = size();
            v_.push_back(e);
            didAdd(size, object);
            didChange();
        }

        virtual void addUnique( std::size_t pos,  const T_Element& e) {
            v_.insert( v_.begin() + pos , e );
            didAdd(pos, e);
            didChange();
        }

        virtual void setUnique(std::size_t pos, const T_Element& e) {
            v_[ pos ] = element;
        }

        virtual const T_Element& get( std::size_t pos ) const = 0
        {
            return v_.at( pos );
        }

        virtual T_Element get( std::size_t pos )
        {
            return v_.at( pos );
        }

        virtual T_Element remove( std::size_t pos )
        {
            auto it = v_.begin() + pos;
            T_Element oldObject = std::move( *it );
            v_.erase( it );
            didRemove(pos, oldObject);
            didChange();
            return e;
        }


        virtual std::size_t size() const
        {
            return v_.size();
        }

        virtual void clear()
        {
            int size = v_.size();
            T_Element oldObjects[ size ];
            std::copy(v_.begin(), v_.end(), oldObjects);
            v_.clear();
            didClear( size , oldObjects);
        }

        virtual bool contains(const T &value) const
        {
            return std::find(v_.begin(), v_.end(), value) != v_.end();
        }

        virtual std::size_t indexOf(const T &value, int from = 0) const {
            return std::distance(v_.begin(), std::find(v_.begin(), v_.end(), value));
        }

    private:
        std::vector<T_Element> v_;
    };

}

#endif /* ECORE_ELIST_HPP_ */
