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

#include "ecore/impl/Notification.hpp"
#include <memory>

namespace
{
    class EObject;
}

namespace ecore::impl
{
    template <typename T>
    class Proxy
    {
    public:
        Proxy()
            : owner_()
            , featureID_(-1)
        {
        }

        Proxy( const std::shared_ptr<EObject>& owner , int featureID )
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

        std::shared_ptr<T> get() const
        {
            if( is_uninitialized( ref_ ) )
            {
                if( proxy_ )
                {
                    auto resolved = owner.eResolveProxy( proxy_ );
                    if( resolved && resolved != proxy_ )
                    {
                        ref_ = resolved;
                        if( isNotificationRequired() )
                        {
                            auto owner = getOwner();
                            _ASSERT( owner );
                            owner->eNotify( std::make_shared< Notification >( owner, featureID_, Notification::REMOVE, proxy_, resolved ) );
                        }
                        return resolved;
                    }
                }
                return proxy_;
            }
            else
                return ref_.lock();
        }

        void set( const std::shared_ptr<T>& ref )
        {
            if( ref->eIsProxy() )
                proxy_ = std::move( ref );
            else
                ref_ = ref;
        }

        Proxy& operator=( const Proxy& o )
        {
            owner_ = o.owner_;
            featureID_ = o.featureID_;
            ref_ = o.ref_;
            proxy_ = o.proxy_;
            return *this;
        }

        Proxy& operator=( const std::shared_ptr<T>& ref )
        {
            set( ref );
            return *this;
        }

    private:
        template <typename T>
        bool is_uninitialized( std::weak_ptr<T> const& weak )
        {
            using wt = std::weak_ptr<T>;
            return !weak.owner_before( wt{} ) && !wt{}.owner_before( weak );
        }

        std::shared_ptr<EObject> getOwner() const
        {
            return owner_.lock();
        }

        bool isNotificationRequired() const
        {
            auto owner = getOwner();
            return owner ? ( owner->eDeliver() && !owner->eAdapters().empty() ) : false;
        }

    private:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        std::weak_ptr<T> ref_;
        std::shared_ptr<T> proxy_;
    };
}

#endif