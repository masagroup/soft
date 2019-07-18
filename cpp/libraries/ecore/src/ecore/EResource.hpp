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

#include "ecore/Exports.hpp"
#include "ecore/ENotifier.hpp"

#include <memory>
#include <string>

namespace ecore
{

    template <typename T>
    class EList;

    template <typename T>
    class ECollectionView;

    class EDiagnostic;
    class EObject;
    class EResourceSet;
    class EUriInputStream;
    class EUriOutputStream;

    class Uri;

    class ECORE_API EResource : public virtual ENotifier
    {
    public:
        /**
         * The {@link #getResourceSet} feature {@link ENotification#getFeatureID ID}.
         */
        static const int RESOURCE__RESOURCE_SET = 0;

        /**
         * The {@link #getURI} feature {@link ENotification#getFeatureID ID}.
         */
        static const int RESOURCE__URI = 1;

        /**
         * The {@link #getContents} feature {@link ENotification#getFeatureID ID}.
         */
        static const int RESOURCE__CONTENTS = 2;

        /**
         * The {@link #isLoaded} feature {@link ENotification#getFeatureID ID}.
         */
        static const int RESOURCE__IS_LOADED = 4;

    public:
        virtual ~EResource() = default;

        virtual std::shared_ptr<EResourceSet> getResourceSet() const = 0;

        virtual const Uri& getUri() const = 0;

        virtual void setUri( const Uri& uri ) = 0;

        virtual std::shared_ptr<EList<std::shared_ptr<EObject>>> getContents() const = 0;

        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> getAllContents() const = 0;

        virtual std::shared_ptr<EObject> getEObject( const std::string& uriFragment ) const = 0;

        virtual void attached( const std::shared_ptr<EObject>& object ) = 0;

        virtual void detached( const std::shared_ptr<EObject>& object ) = 0;

        virtual void load() = 0;

        virtual void load( std::istream& is ) = 0;

        virtual void unload() = 0;

        virtual bool isLoaded() const = 0;

        virtual void save() = 0;

        virtual void save( std::ostream& os )= 0;

        virtual std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> getErrors() const = 0;

        virtual std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> getWarnings() const = 0;

    };

} // namespace ecore

#endif /* ECORE_ERESOURCE_HPP_ */
