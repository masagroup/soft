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
    class EUriConverter;
} // namespace ecore

namespace ecore::impl
{
    class AbstractResource : public virtual AbstractNotifier<EResource>
    {
    public:
        AbstractResource();

        AbstractResource( const Uri& uri );

        virtual ~AbstractResource();

        void setThisPtr( const std::shared_ptr<AbstractResource>& resource );
        std::shared_ptr<AbstractResource> getThisPtr() const;

        virtual std::shared_ptr<EResourceSet> getResourceSet() const;

        virtual const Uri& getUri() const;

        virtual void setUri( const Uri& uri );

        virtual std::shared_ptr<EList<std::shared_ptr<EObject>>> getContents() const;

        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> getAllContents() const;

        virtual void attached( const std::shared_ptr<EObject>& object );

        virtual void detached( const std::shared_ptr<EObject>& object );

        virtual void load();

        virtual void load( std::istream& is );

        virtual void unload();

        virtual bool isLoaded() const;

        virtual void save();

        virtual void save( std::ostream& os );

        std::shared_ptr<ENotificationChain> basicSetLoaded( bool isLoaded,
                                                            const std::shared_ptr<ENotificationChain>& notifications );

        std::shared_ptr<ENotificationChain> basicSetResourceSet(
            const std::shared_ptr<EResourceSet> resourceSet, const std::shared_ptr<ENotificationChain>& notifications );

    protected:
        virtual void doLoad( std::istream& is ) = 0;
        virtual void doSave( std::ostream& os ) = 0;
        virtual void doUnload(); 

    private:
        std::shared_ptr<EUriConverter> getUriConverter() const;
        std::shared_ptr<EList<std::shared_ptr<EObject>>> initContents();

    private:
        class Notification;

    private:
        std::weak_ptr<AbstractResource> thisPtr_;
        std::weak_ptr<EResourceSet> resourceSet_;
        Uri uri_;
        Lazy<std::shared_ptr<EList<std::shared_ptr<EObject>>>> eContents_;
        bool isLoaded_{false};
    };

} // namespace ecore::impl

#endif // ECORE_RESOURCE_HPP_