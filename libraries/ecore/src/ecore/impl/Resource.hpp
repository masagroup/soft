// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_RESOURCE_HPP_
#define ECORE_RESOURCE_HPP_

#include "ecore/EResource.hpp"
#include "ecore/impl/AbstractNotifier.hpp"
#include "ecore/impl/Lazy.hpp"

#include <memory>

namespace ecore::impl
{
    class Resource : public virtual AbstractNotifier< EResource > , private std::enable_shared_from_this<Resource>
    {
    public:
        Resource();

        virtual ~Resource();

        void setThisPtr( const std::shared_ptr<Resource>& resource );
        std::shared_ptr<Resource> getThisPtr() const;

        virtual std::shared_ptr< EList< std::shared_ptr< EObject > > > getContents() const;

        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> getAllContents() const;

        virtual void attached( const std::shared_ptr<EObject>& object );

        virtual void detached( const std::shared_ptr<EObject>& object );

    private:
        virtual std::shared_ptr< EList< std::shared_ptr< EObject > > > initContents() const;

    private:
        std::weak_ptr<Resource> thisPtr_;
        Lazy< std::shared_ptr< EList< std::shared_ptr< EObject > > > > eContents_;
    };

}

#endif // ECORE_RESOURCE_HPP_