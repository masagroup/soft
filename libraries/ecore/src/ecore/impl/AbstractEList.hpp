// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTELIST_HPP_
#define ECORE_ABSTRACTELIST_HPP_

#include "ecore/EList.hpp"
#include "ecore/impl/ImmutableEList.hpp"

#include <vector>
#include<unordered_set>

namespace ecore::impl
{

    template<typename I, bool unique >
    class AbstractEList : public I
    {
    public:
        typedef typename I InterfaceType;
        typedef typename I::ValueType ValueType;
        
        AbstractEList()
        {
        }

        virtual ~AbstractEList()
        {
        }

        virtual bool add( const ValueType& e )
        {
            return doAdd( e );
        }

        virtual bool addAll( const EList<ValueType>& l )
        {
            return doAddAll( l );
        }

        virtual void add( std::size_t pos, const ValueType& e )
        {
            VERIFY( pos <= size() , "out of range" );
            doAdd( pos, e );
        }

        virtual bool addAll( std::size_t pos, const EList<ValueType>& l )
        {
            VERIFY( pos <= size(), "out of range" );
            return doAddAll( pos, l );
        }

        virtual void addUnique( const ValueType& e ) = 0;

        virtual void addUnique( std::size_t pos, const ValueType& e ) = 0;

        virtual bool addAllUnique( const EList<ValueType>& l ) = 0;

        virtual bool addAllUnique( std::size_t pos, const EList<ValueType>& l ) = 0;

        using EList::move;

        virtual void move( std::size_t newPos, const ValueType& e )
        {
            move( newPos, indexOf( e ) );
        }

        virtual void set( std::size_t pos, const ValueType& e )
        {
            VERIFY( pos < size(), "out of range" );
            doSet( pos, e );
        }

        virtual ValueType setUnique( std::size_t pos, const ValueType& e ) = 0;

        virtual bool remove( const ValueType& e )
        {
            std::size_t index = indexOf( e );
            if (index >= 0)
            {
                remove( index );
                return true;
            }
            else
            {
                return false;
            }
        }

        virtual ValueType remove( std::size_t index ) = 0;

        virtual bool empty() const
        {
            return size() == 0;
        }

    protected:

        virtual void didSet( std::size_t pos, const ValueType& newObject, const ValueType& oldObject )
        {
            // Do nothing.
        }

        virtual void didAdd( std::size_t pos, const ValueType& newObject )
        {
            // Do nothing.
        }

        virtual void didRemove( std::size_t pos, const ValueType& oldObject )
        {
            // Do nothing.
        }

        virtual void didClear( const std::vector<ValueType>& oldObjects ) {
            for (int i = 0; i < oldObjects.size(); ++i)
                didRemove( i, oldObjects[i] );
        }

        virtual void didMove( std::size_t pos, const ValueType& movedObject, std::size_t oldIndex )
        {
            // Do nothing.
        }

        virtual void didChange()
        {
            // Do nothing.
        }

    private:
        
        template <bool u = unique>
        typename std::enable_if< u, bool >::type doAdd( const ValueType& e )
        {
            if ( contains( e ))
                return  false;
            else
            {
                addUnique( e );
                return true;
            }
        }

        template <bool u = unique>
        typename std::enable_if< !u, bool >::type doAdd( const ValueType& e )
        {
            addUnique( e );
            return true;
        }

        template <bool u = unique>
        typename std::enable_if< u, void >::type doAdd( std::size_t pos, const ValueType& e )
        {
            VERIFY( !contains( e ) , "element already in list" );
            addUnique( pos, e );
        }

        template <bool u = unique>
        typename std::enable_if < !u, void > ::type doAdd( std::size_t pos, const ValueType& e )
        {
            addUnique( pos, e );
        }

        template <bool u = unique>
        typename std::enable_if< u, bool >::type doAddAll( const EList<ValueType>& l )
        {
            auto nonDuplicates = getNonDuplicates( l );
            return addAllUnique( *nonDuplicates );
        }

        template <bool u = unique>
        typename std::enable_if< !u, bool >::type doAddAll( const EList<ValueType>& l )
        {
            return addAllUnique( l );
        }

        template <bool u = unique>
        typename std::enable_if< u, bool >::type doAddAll( std::size_t pos, const EList<ValueType>& l )
        {
            auto nonDuplicates = getNonDuplicates( l );
            return addAllUnique( pos, *nonDuplicates );
        }

        template <bool u = unique>
        typename std::enable_if< !u, bool >::type doAddAll( std::size_t pos, const EList<ValueType>& l )
        {
            return addAllUnique( pos, l );
        }

        template <bool u = unique>
        typename std::enable_if< u, void >::type doSet( std::size_t pos, const ValueType& e )
        {
            std::size_t currentIndex = indexOf( e );
            VERIFY( currentIndex == -1 || currentIndex == pos , "element already in list");
            setUnique( pos, e );
        }

        template <bool u = unique>
        typename std::enable_if< !u, void >::type doSet( std::size_t pos, const ValueType& e )
        {
            setUnique( pos, e );
        }

    private:
        std::unique_ptr<EList<ValueType>> getNonDuplicates( const EList<ValueType>& l )
        {
            std::unordered_set<ValueType> s;
            std::vector<ValueType> v;
            for (auto e : l)
            {
                auto i = s.insert( e );
                if (i.second)
                {
                    if (!contains( e ))
                        v.push_back( e );
                }
            }
            return std::make_unique<ImmutableEList<ValueType>>( std::move( v ) );
        }

    };

}

#endif /* ECORE_ABSTRACTELIST_HPP_ */
