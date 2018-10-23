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

namespace ecore {

    template <typename T>
    class EList : public std::enable_shared_from_this<EList<T>> {
    public:
        typedef typename std::shared_ptr<EList<T>> T_Ptr_Type;
        typedef typename std::shared_ptr<const EList<T>> T_Ptr_Const_Type;

        virtual ~EList() {}

        virtual bool add( const T& e ) = 0;

        virtual void add( std::size_t pos, const T& e ) = 0;

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

        std::size_t indexOf( const T& e, std::size_t from = 0 ) const {
            return std::distance( begin() + (int)from, std::find( begin(), end(), e ) );
        }

        /** Iterator interfaces for an EList<T>. */
        template <typename EListPtrType>
        class EListIterator : public std::iterator<std::random_access_iterator_tag, T> {
        public:
            EListIterator( EListPtrType eList, std::size_t index )
                : eList_( eList ),
                index_( index ) {
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

            EListIterator operator+( const difference_type& offset )
            {
                EListIterator tmp = *this;
                return (tmp += offset);
            }

            EListIterator operator-( const difference_type& rhs )
            {
                EListIterator tmp = *this;
                return (tmp -= offset);
            }

            difference_type operator-( const EListIterator& rhs )
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

            const EListPtrType& getEList() const {
                return eList_;
            }

            std::size_t getIndex() const {
                return index_;
            }

        private:
#if _ITERATOR_DEBUG_LEVEL == 2
            void _Compat( const EListIterator& rhs ) const
            {	// test for compatible iterator pair
                if (eList_ != rhs.eList_)
                {
                    _ASSERTE( "vector iterators incompatible" && 0 );
                    _SCL_SECURE_INVALID_ARGUMENT;
                }
            }

#elif _ITERATOR_DEBUG_LEVEL == 1
            void _Compat( const EListIterator& rhs ) const
            {	// test for compatible iterator pair
                _SCL_SECURE_VALIDATE_RANGE( eList_ == rhs.eList_ );
            }

#else /* _ITERATOR_DEBUG_LEVEL == 0 */
            void _Compat( const EListIterator& ) const
            {	// test for compatible iterator pair
            }
#endif /* _ITERATOR_DEBUG_LEVEL */

        private:
            EListPtrType eList_;
            std::size_t index_;
        };

        typedef EListIterator<T_Ptr_Type> iterator;
        typedef EListIterator<T_Ptr_Const_Type> const_iterator;

        iterator begin() {
            auto this_shared = std::enable_shared_from_this<EList<T>>::shared_from_this();
            return iterator( this_shared, 0 );
        }

        const_iterator begin() const {
            auto this_shared = std::enable_shared_from_this<EList<T>>::shared_from_this();
            return const_iterator( this_shared, 0 );
        }

        iterator end() {
            auto this_shared = std::enable_shared_from_this<EList<T>>::shared_from_this();
            return iterator( this_shared, size() );
        }

        const_iterator end() const {
            auto this_shared = std::enable_shared_from_this<EList<T>>::shared_from_this();
            return const_iterator( this_shared, size() );
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
        inline typename EList< Q >::T_Ptr_Type asEListOf()
        {
            return std::make_shared<DelegateEList< Q, T >>( *this );
        }

    };

    template< typename T, typename Q >
    class DelegateEList : public EList< T >
    {
    public:

        typedef EList< Q > T_ListDelegate;

        DelegateEList( T_ListDelegate& delegate ) :
            delegate_( delegate )
        {
        }

        virtual ~DelegateEList()
        {
        }

        virtual bool add( const T& e )
        {
            return delegate_.add( cast< T, Q >::do_cast( e ) );
        }

        virtual void add( std::size_t pos, const T& e )
        {
            delegate_.add( pos, cast< T, Q >::do_cast( e ) );
        }

        virtual T get( std::size_t pos ) const {
            return cast< Q, T >::do_cast( delegate_.get( pos ) );
        }

        virtual void set( std::size_t pos, const T& e )
        {
            delegate_.set( pos, cast< T, Q >::do_cast( e ) );
        }

        virtual T remove( std::size_t pos )
        {
            return cast< Q, T >::do_cast( delegate_.remove( pos ) );
        }

        virtual bool remove( const T& e )
        {
            return delegate_.remove( cast< T, Q >::do_cast( e ) );
        }

        virtual std::size_t size() const
        {
            return delegate_.size();
        }

        virtual void clear()
        {
            delegate_.clear();
        }


    protected:

        T_ListDelegate& delegate_;

        template< typename A, typename B >
        struct cast
        {
            static inline B do_cast( const A& a )
            {
                return std::dynamic_pointer_cast<typename B::element_type>(a);
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

}

#endif /* ECORE_ELIST_HPP_ */
