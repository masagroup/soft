// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ELIST_HPP_
#define ECORE_ELIST_HPP_

#include <memory>
#include "ecore/Assert.hpp"


namespace ecore {

    template <typename T>
    class EList : public std::enable_shared_from_this< EList<T> >{
    public:
        typedef typename T ValueType;

        virtual ~EList() {}

        virtual bool add( const T& e ) = 0;

        virtual bool addAll( const EList<T>& l ) = 0;

        virtual void add( std::size_t pos, const T& e ) = 0;

        virtual bool addAll( std::size_t pos, const EList<T>& l ) = 0;

        virtual void move( std::size_t newPos, const T& e ) = 0;

        virtual T move( std::size_t newPos, std::size_t oldPos ) = 0;

        virtual T get( std::size_t pos ) const = 0;

        virtual void set( std::size_t pos, const T& e ) = 0;

        virtual T remove( std::size_t pos ) = 0;

        virtual bool remove( const T& e ) = 0;

        virtual std::size_t size() const = 0;

        virtual void clear() = 0;

        virtual bool empty() const = 0;

        bool contains(const T& e) const {
            return std::find(begin(), end(), e) != end();
        }

        std::size_t indexOf( const T& e ) const {
            std::size_t index = std::distance( begin() , std::find( begin() , end(), e ) );
            return index == size() ? -1 : index;
        }

        /** Iterator interfaces for an EList<T>. */
        template <typename ListType >
        class EListIterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = typename ListType::ValueType;
            using pointer           = typename ListType::ValueType*;
            using reference         = typename ListType::ValueType&;
            
        public:
            EListIterator( ListType* eList, std::size_t index )
                : eList_( eList )
                , index_( index ) {
            }

            T operator*() const {
                return eList_->get( index_ );
            }

            EListIterator& operator--() {
                --index_;
                return *this;
            }

            EListIterator operator--( int ) {
                EListIterator old( *this );
                --(*this);
                return old;
            }

            EListIterator& operator++() {
                ++index_;
                return *this;
            }

            EListIterator operator++( int ) {
                EListIterator old( *this );
                ++(*this);
                return old;
            }

            EListIterator& operator+=( difference_type offset )
            {
                index_ += offset;
                return (*this);
            }

            EListIterator& operator-=( difference_type offset )
            {
                return (*this += -offset);
            }

            EListIterator operator+( const difference_type& offset ) const
            {
                EListIterator tmp = *this;
                return (tmp += offset);
            }

            EListIterator operator-( const difference_type& rhs ) const
            {
                EListIterator tmp = *this;
                return (tmp -= offset);
            }

            difference_type operator-( const EListIterator& rhs ) const
            {
                _Compat( rhs );
                return index_ - rhs.index_;
            }

            bool operator==( const EListIterator& rhs ) const {
                _Compat( rhs );
                return (index_ == rhs.index_);
            }

            bool operator!=( const EListIterator& rhs ) const {
                return !(*this == rhs);
            }

            bool operator<( const EListIterator& rhs )
            {
                _Compat( rhs );
                return index_ < rhs.index_;
            }

            bool operator>( const EListIterator& rhs )
            {
                return (rhs < *this);
            }

            bool operator<=( const EListIterator& rhs )
            {
                return (!(rhs < *this));
            }

            bool operator>=( const EListIterator& rhs )
            {
                return (!(*this < rhs));
            }

            bool hasNext() const {
                return ((int64_t)index_ < (int64_t)eList_->size() - 1);
            }

            const ListType* getEList() const {
                return eList_;
            }

            std::size_t getIndex() const {
                return index_;
            }

        private:
            void _Compat( const EListIterator& rhs ) const
            {
#if _ITERATOR_DEBUG_LEVEL == 0
                (void)rhs;
#else
                VERIFY( eList_ == rhs.eList_, "vector iterators incompatible" );
#endif
            }

        private:
            ListType* eList_;
            std::size_t index_;
        };

        typedef EListIterator<EList<T>> iterator;
        typedef EListIterator<const EList<T>> const_iterator;

        iterator begin() {
            return iterator( this, 0 );
        }

        const_iterator begin() const {
            return const_iterator( this, 0 );
        }

        iterator end() {
            return iterator( this, size() );
        }

        const_iterator end() const {
            return const_iterator( this, size() );
        }

        const_iterator cbegin() const {
            return begin();
        }

        const_iterator cend() const {
            return end();
        }

        /**
         * Allows treating an EList<T> as an EList<Q> (if T can be casted to Q dynamically)
         */
        template< typename Q >
        inline std::shared_ptr<EList< Q >> asEListOf()
        {
            return std::make_shared<DelegateEList<Q, T>>( shared_from_this() );
        }

        template< typename Q >
        inline std::shared_ptr<const EList< Q >> asEListOf() const
        {
            return std::make_shared<ConstDelegateEList<Q, T>>( shared_from_this() );
        }

    };


    template< typename T, typename Q >
    class ConstDelegateEList : public EList< T >
    {
        typedef std::shared_ptr<const EList<Q>> T_ListDelegate;
    public:
        
        ConstDelegateEList( const T_ListDelegate& delegate ):
            delegate_( delegate )
        {
            _ASSERTE( delegate_ );
        }

        virtual ~ConstDelegateEList()
        {
        }

        virtual bool add( const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual void add( std::size_t pos, const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool addAll( const EList<T>& l )
        {
            throw "UnsupportedOperationException";
        }

        virtual bool addAll( std::size_t pos, const EList<T>& l )
        {
            throw "UnsupportedOperationException";
        }

        virtual void move( std::size_t newPos, const T& e )
        {
            throw "UnsupportedOperationException";
        }

        virtual T move( std::size_t newPos, std::size_t oldPos )
        {
            throw "UnsupportedOperationException";
        }

        virtual T get( std::size_t pos ) const
        {
            return cast< Q, T >::do_cast( delegate_->get( pos ) );
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

        virtual std::size_t size() const
        {
            return delegate_->size();
        }

        virtual void clear()
        {
            throw "UnsupportedOperationException";
        }

        virtual bool empty() const
        {
            return delegate_->empty();
        }

    protected:

        T_ListDelegate delegate_;

        template< typename A, typename B >
        struct cast
        {
            static inline B do_cast( const A& a )
            {
                return std::dynamic_pointer_cast<typename B::element_type>( a );
            }
        };

        template< typename A >
        struct cast< A, A >
        {
            static inline A do_cast( const A& a )
            {
                return a;
            }
        };
    };

    template< typename T, typename Q >
    class DelegateEList : public ConstDelegateEList< T , Q >
    {
        typedef std::shared_ptr<EList<Q>> T_ListDelegate;
    public:
        
        DelegateEList( const T_ListDelegate& delegate )
            : ConstDelegateEList< T, Q >( delegate )
            , delegate_(delegate)
        {
            _ASSERTE( delegate_ );
        }

        virtual ~DelegateEList()
        {
        }

        virtual bool add( const T& e )
        {
            return delegate_->add( cast< T, Q >::do_cast( e ) );
        }

        virtual void add( std::size_t pos, const T& e )
        {
            delegate_->add( pos, cast< T, Q >::do_cast( e ) );
        }

        virtual bool addAll( const EList<T>& l )
        {
            auto transformed = const_cast<EList<T>&>( l ).asEListOf<Q>();
            return delegate_->addAll( *transformed );
        }

        virtual bool addAll( std::size_t pos, const EList<T>& l )
        {
            auto transformed = const_cast<EList<T>&>( l ).asEListOf<Q>();
            return delegate_->addAll( pos, *transformed );
        }

        virtual void move( std::size_t newPos, const T& e )
        {
            delegate_->move( newPos, cast<T, Q>::do_cast( e ) );
        }

        virtual T move( std::size_t newPos, std::size_t oldPos )
        {
            return cast< Q, T >::do_cast( delegate_->move( newPos, oldPos ) );
        }

        virtual void set( std::size_t pos, const T& e )
        {
            delegate_->set( pos, cast< T, Q >::do_cast( e ) );
        }

        virtual T remove( std::size_t pos )
        {
            return cast< Q, T >::do_cast( delegate_->remove( pos ) );
        }

        virtual bool remove( const T& e )
        {
            return delegate_->remove( cast< T, Q >::do_cast( e ) );
        }

        virtual void clear()
        {
            delegate_->clear();
        }

    private:
        T_ListDelegate delegate_;
    };

    template <typename T>
    bool operator ==( const EList<T>& lhs, const EList<T>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }

    template <typename T>
    bool operator !=( const EList<T>& lhs, const EList<T>& rhs )
    {
        return !(lhs == rhs);
    }
}

#endif /* ECORE_ELIST_HPP_ */
