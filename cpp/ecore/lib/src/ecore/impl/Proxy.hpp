// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_PROXY_HPP_
#define ECORE_PROXY_HPP_

#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/impl/Notification.hpp"
#include "ecore/impl/TypeTraits.hpp"
#include "ecore/impl/WeakPtr.hpp"
#include <memory>

namespace ecore
{
    class EObject;
}

namespace ecore::impl
{
    template <typename T , typename Enable = void>
    class Proxy
    {
    };

    template <typename T> 
    class Proxy<T, typename std::enable_if<is_shared_ptr<T>::value>::type >
    {
    public:
        Proxy()
            : owner_()
            , featureID_( -1 )
        {
        }

        Proxy( const std::weak_ptr<EObject>& owner, int featureID )
            : owner_( owner )
            , featureID_( featureID )
        {

        }

        Proxy( const std::weak_ptr<EObject>& owner, int featureID , const T& value )
            : owner_( owner )
            , featureID_( featureID )
        {
            set( value );
        }

        Proxy( const Proxy& o )
            : owner_( o.owner_ )
            , featureID_( o.featureID_ )
            , resolved_( o.resolved_ )
            , value_( o.value_ )
        {

        }

        T value() const
        {
            auto resolved = resolved_.lock();
            return resolved ? resolved : value_;
        }

        T get() const
        {
            if( is_uninitialized( resolved_ ) )
            {
                if( value_ )
                {
                    auto owner = owner_.lock();
                    if( owner )
                    {
                        auto resolved = std::static_pointer_cast<typename T::element_type>( owner->getInternal().eResolveProxy( value_ ) );
                        if( resolved && resolved != value_ )
                        {
                            resolved_ = resolved;
                            if( isNotificationRequired() )
                                owner->eNotify(
                                    std::make_shared<Notification>( owner, Notification::RESOLVE, featureID_, value_, resolved ) );
                            return resolved;
                        }
                    }
                }
                return value_;
            }
            else
                return resolved_.lock();
        }

        void set( const T& value )
        {
            if( value )
            {
                if( value->eIsProxy() )
                {
                    resolved_.reset();
                    value_ = std::move( value );
                }
                else
                {
                    resolved_ = value;
                    value_.reset();
                }
            }
            else
            {
                resolved_.reset();
                value_.reset();
            }
        }

        explicit operator bool() const
        {
            return static_cast<bool>(is_uninitialized( resolved_ ) ? value_ : resolved_.lock());
        }

        typename T::element_type* operator->() const
        {   
            return is_uninitialized( resolved_ ) ? value_.operator ->() : resolved_.lock().operator ->();
        }

        Proxy& operator=( const Proxy& o )
        {
            owner_ = o.owner_;
            featureID_ = o.featureID_;
            resolved_ = o.resolved_;
            value_ = o.value_;
            return *this;
        }

        Proxy& operator=( const T& value )
        {
            set( value );
            return *this;
        }

        bool operator ==( const Proxy& o ) const
        {
            return equals( owner_, o.owner_ )
                && featureID_ == o.featureID_
                && equals( resolved_, o.resolved_ )
                && value_ == o.value_;
        }

        bool operator !=( const Proxy& o ) const
        {
            return !operator ==( o );
        }

        bool operator==( const T& value ) const
        {
            return value_ == value;
        }

        bool operator!=( const T& value ) const
        {
            return !operator==( value );
        }

    private:
        
        template <typename T, typename U>
        inline bool equals( const std::weak_ptr<T>& t, const std::weak_ptr<U>& u ) const
        {
            return ( is_uninitialized( t ) && is_uninitialized( u ) ) || ( !is_uninitialized( t ) && t.lock() == u.lock() );
        }

        bool isNotificationRequired() const
        {
            auto owner = owner_.lock();
            return owner ? ( owner->eDeliver() && !owner->eAdapters().empty() ) : false;
        }

    private:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        mutable std::weak_ptr<typename T::element_type> resolved_;
        T value_;
    };

    template<typename T>
    bool operator==( std::nullptr_t, const Proxy<T>& right ) noexcept
    {	// test if nullptr == shared_ptr
        return ( nullptr == right.value() );
    }

    template<typename T>
    bool operator==( const Proxy<T>& left, nullptr_t ) noexcept
    {	// test if nullptr == shared_ptr
        return ( left.value() == nullptr );
    }

    template<typename T>
    bool operator!=( const Proxy<T>& left, std::nullptr_t right ) noexcept
    {	// test if shared_ptr != nullptr
        return ( !( left == right ) );
    }

    template<typename T>
    bool operator!=( std::nullptr_t left, const Proxy<T>& right ) noexcept
    {	// test if nullptr != shared_ptr
        return ( !( left == right ) );
    }
}

#endif

