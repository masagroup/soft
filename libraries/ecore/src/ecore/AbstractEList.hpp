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

namespace ecore
{

    template<typename T , bool unique = false >
    class AbstractEList : public EList<T>
    {

    public:

        AbstractEList()
            : uniquePolicy_( *this )
        {
        }

        virtual ~AbstractEList()
        {
        }

        virtual bool add( const T& e )
        {
            return uniquePolicy_.add( e );
        }

        virtual void addUnique( const T& e ) = 0;

        virtual void add( std::size_t pos, const T& e )
        {
            _SCL_SECURE_ALWAYS_VALIDATE_RANGE( pos <= size() );
            uniquePolicy_.add( pos, e );
        }

        virtual void addUnique( std::size_t pos, const T& e ) = 0;

        virtual void addAll( const T_Ptr_Type& other )
        {

        }

        //virtual void addAllUnique( const T_Ptr_Type& other ) = 0;

        virtual void addAll( std::size_t pos, const T_Ptr_Type& other )
        {

        }

        //virtual void addAllUnique( std::size_t pos, const T_Ptr_Type& other ) = 0;

        virtual void set( std::size_t pos, const T& e )
        {
            _SCL_SECURE_ALWAYS_VALIDATE_RANGE( pos <= size() );
            uniquePolicy_.set( pos, e );
        }

        virtual T setUnique( std::size_t pos, const T& e ) = 0;

        virtual bool remove( const T& e )
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

        virtual T remove( std::size_t index ) = 0;

        virtual bool empty() const
        {
            return size() == 0;
        }

    protected:

        template <bool unique = false> 
        struct UniquePolicy
        {
            inline UniquePolicy( AbstractEList& list ) : list_(list) {}

            inline bool add( const T& e )
            {
                list_.addUnique( e );
                return true;
            }

            inline void add( std::size_t pos, const T& e )
            {
                list_.addUnique( pos, e );
            }

            inline void set( std::size_t pos, const T& e )
            {
                list_.setUnique( pos, e );
            }

            AbstractEList& list_;
        };

        template <>
        struct UniquePolicy<true>
        {
            inline UniquePolicy( AbstractEList& list ) : list_( list ) {}

            inline bool add( const T& e )
            {
                if (list_.contains( e ))
                    return  false;
                else
                {
                    list_.addUnique( e );
                    return true;
                }
            }

            inline void add( std::size_t pos, const T& e )
            {
                _SCL_SECURE_ALWAYS_VALIDATE( !list_.contains( e ) );
                list_.addUnique( pos, e );
            }

            inline void set( std::size_t pos, const T& e )
            {
                std::size_t currentIndex = list_.indexOf( e );
                _SCL_SECURE_ALWAYS_VALIDATE(currentIndex == -1 || currentIndex == pos );
                list_.setUnique( pos, e );
            }

            AbstractEList& list_;
        };


        virtual T primitiveGet( std::size_t pos ) const = 0;

        virtual void didSet( std::size_t pos, const T& newObject, const T& oldObject )
        {
            // Do nothing.
        }

        virtual void didAdd( std::size_t pos, const T& newObject )
        {
            // Do nothing.
        }

        virtual void didRemove( std::size_t pos, const T& oldObject )
        {
            // Do nothing.
        }

        virtual void didClear( const std::vector<T>& oldObjects ) {
            for (int i = 0; i < oldObjects.size(); ++i)
                didRemove( i, oldObjects[i] );
        }

        virtual void didMove( std::size_t pos, const T& movedObject, std::size_t oldIndex )
        {
            // Do nothing.
        }

        virtual void didChange()
        {
            // Do nothing.
        }

    private:
        UniquePolicy<unique> uniquePolicy_;
    };

}

#endif /* ECORE_ABSTRACTELIST_HPP_ */
