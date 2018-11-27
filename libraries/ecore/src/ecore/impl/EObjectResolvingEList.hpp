// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTRESOLVINGELIST_HPP_
#define ECORE_EOBJECTRESOLVINGELIST_HPP_

#include "ecore/ENotifyingList.hpp"
#include "ecore/EUnsettableList.hpp"
#include "ecore/impl/AbstractEList.hpp"
#include "ecore/impl/AbstractEObjectEList.hpp"
#include "ecore/impl/Proxy.hpp"
#include "ecore/impl/TypeTraits.hpp"

#ifdef SHOW_DELETION
#include <iostream>
#endif

namespace ecore::impl
{
    template <typename T, bool containement = false, bool inverse = false, bool opposite = false, bool unset = false >
    class EObjectResolvingEList : public AbstractEObjectEList< AbstractEList<T, typename std::conditional<unset, EUnsettableList<T>, ENotifyingList<T>>::type, true >
        , containement
        , inverse
        , opposite>
    {
        static_assert( is_shared_ptr<T>::value,"EObjectResolvingEList requires std::shared_ptr" );
    public:
        using EList<T>::add;
        using ENotifyingList<T>::add;

        EObjectResolvingEList( const std::shared_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : AbstractEObjectEList( owner, featureID, inverseFeatureID )
        {
        }

        virtual ~EObjectResolvingEList()
        {
        }

        virtual T get( std::size_t pos ) const
        {
            return v_[ pos ].get();
        }

        virtual std::size_t size() const
        {
            return v_.size();
        }

        virtual void clear()
        {
            return v_.clear();
        }

        virtual bool empty() const
        {
            return v_.empty();
        }


    protected:

        virtual void doAddUnique( const T& e )
        {
            std::size_t pos = size();
            v_.push_back( Proxy<T>( owner_.lock(), featureID_, e) );
            didAdd( pos, e );
            didChange();
        }

        virtual void doAddUnique( std::size_t pos, const T& e )
        {
            v_.insert( v_.begin() + pos, Proxy<T>( owner_.lock(), featureID_, e ) );
            didAdd( pos, e );
            didChange();
        }

        virtual bool doAddAllUnique( std::size_t pos, const EList<T>& l )
        {
            std::size_t growth = l.size();
            std::size_t oldSize = v_.size();
            v_.resize( oldSize + growth );
            for( int i = (int)oldSize - 1; i >= (int)pos; --i )
                v_[ i + growth ] = v_[ i ];
            for( int i = 0; i < growth; ++i )
            {
                auto t = l.get( i );
                v_[ i + pos ] = Proxy<T>( owner_.lock(), featureID_, t );
                didAdd( i + pos, t );
                didChange();
            }
            return growth != 0;
        }

        virtual T doSetUnique( std::size_t pos, const T& e )
        {
            auto oldProxy = v_[ pos ];
            auto oldValue = oldProxy.get();
            v_[ pos ] = Proxy<T>( owner_.lock(), featureID_, e );
            didSet( pos, e, oldValue );
            didChange();
            return oldValue;
        }

        virtual T doRemove( std::size_t pos )
        {
            _SCL_SECURE_ALWAYS_VALIDATE_RANGE( pos < size() );
            auto it = v_.begin() + pos;
            auto oldProxy = std::move( *it );
            auto oldValue = oldProxy.get();
            v_.erase( it );
            didRemove( pos, oldValue );
            didChange();
            return oldValue;
        }

    private:
        std::vector< Proxy<T> > v_;
    };

}

#endif
