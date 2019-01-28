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
#include "ecore/Uri.hpp"
#include "ecore/impl/AbstractNotifier.hpp"
#include "ecore/impl/Lazy.hpp"

#include <memory>

namespace ecore
{
    class ENotificationChain;
    
    class EResourceSet;
}

namespace ecore::impl
{
    class Resource : public virtual AbstractNotifier< EResource >
    {
    public:
        Resource();

        virtual ~Resource();

        void setThisPtr( const std::shared_ptr<Resource>& resource );
        std::shared_ptr<Resource> getThisPtr() const;

        virtual std::shared_ptr<EResourceSet> getResourceSet() const;

        virtual const Uri& getUri() const;

        virtual void setUri( const Uri& uri );

        virtual std::shared_ptr< EList< std::shared_ptr< EObject > > > getContents() const;

        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> getAllContents() const;

        virtual void attached( const std::shared_ptr<EObject>& object );

        virtual void detached( const std::shared_ptr<EObject>& object );

        virtual void load();

        virtual void load( std::istream& is );

        virtual void unload();

        virtual bool isLoaded() const;

        virtual void save();

        virtual void save( std::ostream& os );

        std::shared_ptr<ENotificationChain> basicSetResourceSet( const std::shared_ptr<EResourceSet> resourceSet
                                                               , const std::shared_ptr<ENotificationChain>& notifications );

    private:
        virtual std::shared_ptr< EList< std::shared_ptr< EObject > > > initContents();

    private:
        class Notification;

    private:
        std::weak_ptr<Resource> thisPtr_;
        std::weak_ptr<EResourceSet> resourceSet_;
        Uri uri_;
        Lazy< std::shared_ptr< EList< std::shared_ptr< EObject > > > > eContents_;
        bool loaded_{false};
    };

}

#endif // ECORE_RESOURCE_HPP_