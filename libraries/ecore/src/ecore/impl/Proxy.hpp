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

#include "ecore/EObject.hpp"
#include "ecore/impl/Notification.hpp"
#include <memory>

namespace ecore
{
    class EObject;
}

namespace ecore::impl
{
    template<typename T> struct is_shared_ptr : std::false_type
    {
    };

    template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
    {
    };

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

        Proxy( const std::shared_ptr<EObject>& owner, int featureID )
            : owner_( owner )
            , featureID_( featureID )
        {

        }

        Proxy( const Proxy& o )
            : owner_( o.owner_ )
            , featureID_( o.featureID_ )
            , ref_( o.ref_ )
            , proxy_( o.proxy_ )
        {

        }

        T get() const
        {
            if( is_uninitialized( ref_ ) )
            {
                if( proxy_ )
                {
                    auto owner = owner_.lock();
                    if( owner )
                    {
                        auto resolved = std::dynamic_pointer_cast<typename T::element_type>( owner->eResolveProxy( proxy_ ) );
                        if( resolved && resolved != proxy_ )
                        {
                            ref_ = resolved;
                            if( isNotificationRequired() )
                                owner->eNotify( std::make_shared< Notification >( owner, Notification::REMOVE, featureID_, proxy_, resolved ) );
                            return resolved;
                        }
                    }
                }
                return proxy_;
            }
            else
                return ref_.lock();
        }

        void set( const T& ref )
        {
            if( ref )
            {
                if( ref->eIsProxy() )
                    proxy_ = std::move( ref );
                else
                    ref_ = ref;
            }
            else
            {
                ref_.reset();
                proxy_.reset();
            }
        }

        Proxy& operator=( const Proxy& o )
        {
            owner_ = o.owner_;
            featureID_ = o.featureID_;
            ref_ = o.ref_;
            proxy_ = o.proxy_;
            return *this;
        }

        Proxy& operator=( const T& ref )
        {
            set( ref );
            return *this;
        }

        bool operator ==( const Proxy& o ) const
        {
            return equals( owner_, o.owner_ )
                && featureID_ == o.featureID_
                && equals( ref_, o.ref_ )
                && proxy_ == o.proxy_;
        }

        bool operator !=( const Proxy& o ) const
        {
            return !operator ==( o );
        }

        T getNoResolution() const
        {
            auto ref = ref_.lock();
            return ref ? ref : proxy_;
        }

    private:
        template <typename T>
        bool is_uninitialized( const std::weak_ptr<T>& weak ) const
        {
            using wt = std::weak_ptr<T>;
            return !weak.owner_before( wt{} ) && !wt{}.owner_before( weak );
        }

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
        mutable std::weak_ptr<typename T::element_type> ref_;
        T proxy_;
    };

    template<typename T>
    bool operator==( std::nullptr_t, const Proxy<T>& right ) _NOEXCEPT
    {	// test if nullptr == shared_ptr
        return ( nullptr == right.getNoResolution() );
    }

    template<typename T>
    bool operator==( const Proxy<T>& left, nullptr_t ) _NOEXCEPT
    {	// test if nullptr == shared_ptr
        return ( left.getNoResolution() == nullptr );
    }

    template<typename T>
    bool operator!=( const Proxy<T>& left, std::nullptr_t right ) _NOEXCEPT
    {	// test if shared_ptr != nullptr
        return ( !( left == right ) );
    }

    template<typename T>
    bool operator!=( std::nullptr_t left, const Proxy<T>& right ) _NOEXCEPT
    {	// test if nullptr != shared_ptr
        return ( !( left == right ) );
    }
}

#endif

