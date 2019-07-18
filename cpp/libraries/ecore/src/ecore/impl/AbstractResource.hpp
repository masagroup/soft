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

#include "ecore/Exports.hpp"
#include "ecore/EResource.hpp"
#include "ecore/Uri.hpp"
#include "ecore/impl/AbstractNotifier.hpp"
#include "ecore/impl/Lazy.hpp"

#include <memory>

namespace ecore
{
    class ENotificationChain;
    class EResourceSet;
    class EStructuralFeature;
    class EUriConverter;
} // namespace ecore

namespace ecore::impl
{
    class ECORE_API AbstractResource : public virtual AbstractNotifier<EResource>
    {
    public:
        AbstractResource();

        AbstractResource( const Uri& uri );

        virtual ~AbstractResource();

        virtual std::shared_ptr<EResourceSet> getResourceSet() const;

        virtual const Uri& getUri() const;

        virtual void setUri( const Uri& uri );

        virtual std::shared_ptr<EList<std::shared_ptr<EObject>>> getContents() const;

        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> getAllContents() const;

        virtual std::shared_ptr<EObject> getEObject( const std::string& uriFragment ) const;

        virtual void attached( const std::shared_ptr<EObject>& object );

        virtual void detached( const std::shared_ptr<EObject>& object );

        virtual void load();

        virtual void load( std::istream& is );

        virtual void unload();

        virtual bool isLoaded() const;

        virtual void save();

        virtual void save( std::ostream& os );

        virtual std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> getErrors() const;

        virtual std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> getWarnings() const;

        std::shared_ptr<ENotificationChain> basicSetLoaded( bool isLoaded, const std::shared_ptr<ENotificationChain>& notifications );

        std::shared_ptr<ENotificationChain> basicSetResourceSet( const std::shared_ptr<EResourceSet> resourceSet,
                                                                 const std::shared_ptr<ENotificationChain>& notifications );

    protected:
        virtual void doLoad( std::istream& is ) = 0;
        virtual void doSave( std::ostream& os ) = 0;
        virtual void doUnload();

    private:
        std::shared_ptr<EUriConverter> getUriConverter() const;
        std::shared_ptr<EList<std::shared_ptr<EObject>>> initContents();
        std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> initDiagnostics();

        std::shared_ptr<EObject> getObjectByPath( const std::vector<std::string_view>& uriFragmentPath ) const;
        std::shared_ptr<EObject> getObjectByID( const std::string& id ) const;
        std::shared_ptr<EObject> getObjectForRootSegment( const std::string& rootSegment ) const;
        std::shared_ptr<EObject> getObjectForFragmentSegment( const std::shared_ptr<EObject>& eObject, const std::string& uriSegment ) const;
        std::shared_ptr<EStructuralFeature> getStructuralFeature( const std::shared_ptr<EObject>& eObject, const std::string& name ) const;

    private:
        class Notification;

    private:
        std::weak_ptr<EResourceSet> resourceSet_;
        Uri uri_;
        Lazy<std::shared_ptr<EList<std::shared_ptr<EObject>>>> eContents_{[&]() { return initContents(); }};
        Lazy<std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>>> errors_{[&]() { return initDiagnostics(); }};
        Lazy<std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>>> warnings_{[&]() { return initDiagnostics(); }};
        bool isLoaded_{false};
    };

} // namespace ecore::impl

#endif // ECORE_RESOURCE_HPP_