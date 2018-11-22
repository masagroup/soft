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
        Proxy( ecore::EObject& owner )
            : owner_( owner )
        {

        }

        std::shared_ptr<T> get() const
        {
            if( is_uninitialized( ref_ ) )
            {
                if( proxy_ )
                {
                    throw "ProxyResolutionToBeImplemented";
                }
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

    private:
        EObject& owner_;
        std::weak_ptr<T> ref_;
        std::shared_ptr<T> proxy_;
    };
}

#endif