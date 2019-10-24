// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ERESOURCE_SET_HPP_
#define ECORE_ERESOURCE_SET_HPP_

#include "ecore/Exports.hpp"
#include "ecore/ENotifier.hpp"

#include <memory>

namespace ecore
{
    class EObject;
    class EResource;
    class EResourceFactoryRegistry;
    class URIConverter;
    class URI;

    class ECORE_API EResourceSet : public virtual ENotifier
    {
    public:
        virtual ~EResourceSet() = default;

        virtual std::shared_ptr<EResource> createResource(const URI& uri) = 0;
        virtual std::shared_ptr<EList<std::shared_ptr<EResource>>> getResources() const = 0;
        virtual std::shared_ptr<EResource> getResource(const URI& uri, bool loadOnDemand) = 0;

        virtual std::shared_ptr<EObject> getEObject(const URI& uri, bool loadOnDemand) = 0;

        virtual std::shared_ptr<URIConverter> getURIConverter() const = 0;
        virtual void setURIConverter( const std::shared_ptr<URIConverter>& uriConverter ) = 0;

        virtual std::shared_ptr<EResourceFactoryRegistry> gerResourceFactoryRegistry() const = 0;
        virtual void setResourceFactoryRegistry( const std::shared_ptr<EResourceFactoryRegistry>& resourceFactoryRegistry ) = 0;
    };

} // namespace ecore

#endif
