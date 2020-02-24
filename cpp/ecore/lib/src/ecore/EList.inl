// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ELIST_HPP_
#error This file may only be included from EList.hpp
#endif

#include "ecore/EList.hpp"
#include "ecore/Any.hpp"

namespace ecore
{
    template <typename T>
    bool operator==( const EList<T>& lhs, const EList<T>& rhs )
    {
        return lhs.size() == rhs.size() && std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }

    template <typename T>
    bool operator!=( const EList<T>& lhs, const EList<T>& rhs )
    {
        return !( lhs == rhs );
    }

    namespace detail
    {
        template <typename T, typename Q>
        class ConstDelegateEList : public EList<T>
        {
            typedef std::shared_ptr<const EList<Q>> T_ListDelegate;

        public:
            ConstDelegateEList( const T_ListDelegate& delegate )
                : delegate_( delegate )
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
                return cast<Q, T>::do_cast( delegate_->get( pos ) );
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

            template <typename A, typename B>
            struct cast
            {
                static inline B do_cast( const A& a )
                {
                    return std::static_pointer_cast<typename B::element_type>( a );
                }
            };

            template <typename A>
            struct cast<A, A>
            {
                static inline A do_cast( const A& a )
                {
                    return a;
                }
            };

            template <typename A>
            struct cast<A,Any>
            {
                static inline Any do_cast( const A& a )
                {
                    return a;
                }
            };

            template <typename A>
            struct cast<Any, A>
            {
                static inline A do_cast( const Any& a )
                {
                    return anyCast<A>(a);
                }
            };
        };

        template <typename T, typename Q>
        class DelegateEList : public ConstDelegateEList<T, Q>
        {
            typedef std::shared_ptr<EList<Q>> T_ListDelegate;

        public:
            DelegateEList( const T_ListDelegate& delegate )
                : ConstDelegateEList<T, Q>( delegate )
                , delegate_( delegate )
            {
                _ASSERTE( delegate_ );
            }

            virtual ~DelegateEList()
            {
            }

            virtual bool add( const T& e )
            {
                return delegate_->add( cast<T, Q>::do_cast( e ) );
            }

            virtual void add( std::size_t pos, const T& e )
            {
                delegate_->add( pos, cast<T, Q>::do_cast( e ) );
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
                return cast<Q, T>::do_cast( delegate_->move( newPos, oldPos ) );
            }

            virtual void set( std::size_t pos, const T& e )
            {
                delegate_->set( pos, cast<T, Q>::do_cast( e ) );
            }

            virtual T remove( std::size_t pos )
            {
                return cast<Q, T>::do_cast( delegate_->remove( pos ) );
            }

            virtual bool remove( const T& e )
            {
                return delegate_->remove( cast<T, Q>::do_cast( e ) );
            }

            virtual void clear()
            {
                delegate_->clear();
            }

        private:
            T_ListDelegate delegate_;
        };
    }
} // namespace ecore
