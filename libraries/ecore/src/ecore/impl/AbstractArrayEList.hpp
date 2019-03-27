// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTARRAYELIST_HPP_
#define ECORE_ABSTRACTARRAYELIST_HPP_

#include "ecore/impl/AbstractEList.hpp"

#include <vector>
#include <functional>
#include <type_traits>

namespace ecore::impl
{

    template <typename I, typename S, bool unique >
    class AbstractArrayEList : public AbstractEList<I, unique >
    {
    public:
        typedef typename I InterfaceType;
        typedef typename I::ValueType ValueType;
        typedef typename S StorageType;

        template <typename = std::enable_if< std::is_same<ValueType, StorageType>::value>::type>
        AbstractArrayEList()
            : AbstractEList<I, unique >()
            , from_( identity() )
            , to_( identity() )
            , v_()
        {
        }

        template <typename = std::enable_if< std::is_same<ValueType, StorageType>::value>::type>
        AbstractArrayEList( const std::vector<ValueType>&& v)
            : AbstractEList<I, unique >()
            , from_( identity() )
            , to_( identity() )
            , v_(v)
        {
        }

        template <typename = std::enable_if< std::is_same<ValueType, StorageType>::value>::type>
        AbstractArrayEList( const std::initializer_list<ValueType>& init )
            : AbstractEList<I, unique >()
            , from_( identity() )
            , to_( identity() )
            , v_( init )
        {
        }

        template <typename = std::enable_if< !std::is_same<ValueType, StorageType>::value>::type>
        AbstractArrayEList( std::function< ValueType( const StorageType& )> from
                          , std::function< StorageType( const ValueType& )> to )
            : AbstractEList<I, unique >()
            , from_( from )
            , to_( to )
            , v_()
        {
        }

        AbstractArrayEList( const AbstractArrayEList<I, S, unique>& o )
            : AbstractEList<I, S, unique >( o )
            , from_( o.from_ )
            , to_( o.to_ )
            , v_( o.v_ )
        {
        }

        virtual ~AbstractArrayEList()
        {
        }

        virtual void addUnique( const ValueType& e )
        {
            std::size_t pos = size();
            v_.push_back( to_( e ) );
            didAdd( pos, e );
            didChange();
        }

        virtual void addUnique( std::size_t pos, const ValueType& e )
        {
            v_.insert( v_.begin() + pos, to_( e ) );
            didAdd( pos, e );
            didChange();
        }

        virtual bool addAllUnique( const EList<ValueType>& l )
        {
            std::size_t growth = l.size();
            std::size_t oldSize = v_.size();
            v_.resize( oldSize + growth );
            for( int i = 0; i < growth; ++i )
            {
                auto v = l.get( i );
                v_[ i + oldSize ] = to_( v );
                didAdd( i + oldSize, v );
                didChange();
            }
            return growth != 0;
        }

        virtual bool addAllUnique( std::size_t pos, const EList<ValueType>& l )
        {
            std::size_t growth = l.size();
            std::size_t oldSize = v_.size();
            v_.resize( oldSize + growth );
            for( int i = (int)oldSize - 1; i >= (int)pos; --i )
                v_[ i + growth ] = v_[ i ];
            for( int i = 0; i < growth; ++i )
            {
                auto v = l.get( i );
                v_[ i + pos ] = to_( v );
                didAdd( i + pos, v );
                didChange();
            }
            return growth != 0;
        }

        virtual ValueType setUnique( std::size_t pos, const ValueType& e )
        {
            auto old = from_( v_[ pos ] );
            v_[ pos ] = to_( e );
            didSet( pos, e, old );
            didChange();
            return old;
        }

        virtual ValueType get( std::size_t pos ) const
        {
            return from_( v_[ pos ] );
        }


        virtual ValueType move( std::size_t newPos, std::size_t oldPos )
        {
            VERIFY( newPos < size(), "newPos : out of range" );
            VERIFY( oldPos < size(), "oldPos : out of range" );
            auto it = v_.begin() + oldPos;
            auto element = std::move( *it );
            auto object = from_( element );
            v_.erase( it );
            v_.insert( v_.begin() + newPos, element );
            didMove( newPos, object, oldPos );
            didChange();
            return object;
        }

        virtual ValueType remove( std::size_t pos )
        {
            VERIFY( pos < size() , "out of range");
            auto it = v_.begin() + pos;
            auto oldObject = from_( std::move( *it ) );
            v_.erase( it );
            didRemove( pos, oldObject );
            didChange();
            return oldObject;
        }


        virtual std::size_t size() const
        {
            return v_.size();
        }

        virtual bool empty() const
        {
            return v_.empty();
        }

        virtual void clear()
        {
            doClear();
        }

        std::vector<StorageType>& data()
        {
            return v_;
        }

    private:
        struct identity
        {
            template<typename U>
            constexpr auto operator()( U&& v ) const noexcept
                -> decltype(std::forward<U>( v ))
            {
                return std::forward<U>( v );
            }
        };

        template <typename T = ValueType>
        typename std::enable_if< std::is_same<T, StorageType>::value >::type doClear()
        {
            auto oldObjects = std::move( v_ );
            v_.clear();
            didClear( oldObjects );
        }

        template <typename T = ValueType>
        typename std::enable_if< !std::is_same<T, StorageType>::value >::type doClear()
        {
            auto oldObjects = std::move( v_ );
            v_.clear();
            std::vector<ValueType> oldValues;
            std::transform( oldObjects.begin(), oldObjects.end(), oldValues.end(), from_ );
            didClear( oldValues );
        }

    private:
        std::function< StorageType( const ValueType& )> to_;
        std::function< ValueType( const StorageType& )> from_;
        std::vector<StorageType> v_;
    };

}

#endif /* ECORE_ABSTRACTARRAYELIST_HPP_ */
