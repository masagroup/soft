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

#include "ecore/impl/AbstractEList.hpp"

#include <vector>


namespace ecore::impl
{

    template <typename T, bool unique = false>
    class BasicEList : public AbstractEList<T,unique>
    {
    public:
        BasicEList()
            : AbstractEList<T, unique>()
            , v_()
        {

        }

        BasicEList( std::initializer_list<T> init )
            : AbstractEList<T, unique>()
            , v_( init )
        {

        }


        BasicEList( const BasicEList<T>& o )
            : AbstractEList<T, unique>( o )
            , v_( o.v_ )
        {

        }

        virtual ~BasicEList()
        {
        }

        virtual void addUnique( const T& e )
        {
            std::size_t pos = size();
            v_.push_back( e );
            didAdd( pos, e );
            didChange();
        }

        virtual void addUnique( std::size_t pos, const T& e )
        {
            v_.insert( v_.begin() + pos, e );
            didAdd( pos, e );
            didChange();
        }

        virtual bool addAllUnique( const std::shared_ptr<EList<T>>& l )
        {
            std::size_t growth = l->size();
            std::size_t oldSize = v_.size();
            v_.resize( oldSize + growth );
            for (int i = 0; i < growth; ++i)
            {
                auto t = l->get( i );
                v_[i + oldSize] = t;
                didAdd( i + oldSize, t );
                didChange();
            }
            return growth != 0 ;
        }

        virtual bool addAllUnique( std::size_t pos,  const std::shared_ptr<EList<T>>& l )
        {
            std::size_t growth = l->size();
            std::size_t oldSize = v_.size();
            v_.resize( oldSize + growth );
            for (int i = (int)oldSize -1 ; i >= (int)pos; --i)
                v_[ i + growth ] = v_[i];
            for (int i = 0; i < growth; ++i)
            {
                auto t = l->get( i );
                v_[i + pos] = t;
                didAdd( i + pos, t );
                didChange();
            }
            return growth != 0;
        }

        virtual T setUnique( std::size_t pos, const T& e )
        {
            T old = v_[ pos ];
            v_[ pos ] = e;
            didSet( pos, e, old );
            didChange();
            return old;
        }

        virtual T get( std::size_t pos ) const
        {
            return v_[pos];
        }

        virtual T remove( std::size_t pos )
        {
            _SCL_SECURE_ALWAYS_VALIDATE_RANGE( pos < size() );
            auto it = v_.begin() + pos;
            T oldObject = std::move( *it );
            v_.erase( it );
            didRemove( pos, oldObject );
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
            didClear( oldObjects );
        }

        virtual bool empty() const
        {
            return v_.empty();
        }

    protected:
        std::vector<T> v_;
    };

}

#endif /* ECORE_ELIST_HPP_ */
