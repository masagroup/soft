// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ERESOURCE_HPP_
#define ECORE_ERESOURCE_HPP_

#include "ecore/ENotifier.hpp"

#include <memory>

namespace ecore
{

    template <typename T > class EList;

    template <typename T > class ECollectionView;

    class EObject;

    class EResource : public virtual ENotifier
    {
    public:
        virtual ~EResource() = default;

        virtual std::shared_ptr< EList< std::shared_ptr< EObject > > > getContents() const = 0;

        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> getAllContents() const = 0;

        virtual void attached( const std::shared_ptr<EObject>& object ) = 0;

        virtual void detached( const std::shared_ptr<EObject>& object ) = 0;
    };

}

#endif /* ECORE_ERESOURCE_HPP_ */
