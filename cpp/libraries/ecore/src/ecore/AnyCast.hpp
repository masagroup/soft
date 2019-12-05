// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ANYCAST_HPP_
#define ECORE_ANYCAST_HPP_

#include "ecore/Any.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EList.hpp"


namespace ecore
{
    template <typename T>
    T anyObjectCast( const Any& any )
    {
        auto id = &any.type();
        if( id == &typeid( T ) )
            return anyCast<T>( any );
        else if( id == &typeid( std::shared_ptr<EObject> ) )
        {
            auto object = anyCast<std::shared_ptr<EObject>>( any );
            return std::dynamic_pointer_cast<typename T::element_type>( object );
        }
        return nullptr;
    }

    template <typename T>
    std::shared_ptr<EList<T>> anyListCast( const Any& any )
    {
        auto id = &any.type();
        if( id == &typeid( std::shared_ptr<EList<T>> ) )
            return anyCast<std::shared_ptr<EList<T>>>( any );
        else if( id == &typeid( std::shared_ptr<EList<std::shared_ptr<EObject>>> ) )
        {
            auto l = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>( any );
            return l->asEListOf<T>();
        }
        else if( id == &typeid( std::shared_ptr<EList<Any>> ) )
        {
            auto l = anyCast<std::shared_ptr<EList<Any>>>( any );
            return l->asEListOf<T>();
        }
        return nullptr;
    }

} // namespace ecore

#endif /* ECORE_ANYCAST_HPP_ */
