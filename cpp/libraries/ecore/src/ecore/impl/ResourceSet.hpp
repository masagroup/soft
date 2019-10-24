// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_RESOURCE_SET_HPP_
#define ECORE_RESOURCE_SET_HPP_

#include "ecore/Exports.hpp"
#include "ecore/EResourceSet.hpp"
#include "ecore/impl/AbstractNotifier.hpp"
#include "ecore/impl/Lazy.hpp"

namespace ecore::impl
{
    class ECORE_API ResourceSet : public virtual AbstractNotifier< EResourceSet >
    {
    public:
        ResourceSet();

        virtual ~ResourceSet();

        virtual std::shared_ptr<EResource> createResource(const URI& uri) const;

        virtual std::shared_ptr<EList<std::shared_ptr<EResource>>> getResources() const;
   
        virtual std::shared_ptr<URIConverter> getURIConverter() const;
        virtual void setURIConverter( const std::shared_ptr<URIConverter>& uriConverter );

        virtual std::shared_ptr<EResourceFactoryRegistry> gerResourceFactoryRegistry() const;
        virtual void setResourceFactoryRegistry( const std::shared_ptr<EResourceFactoryRegistry>& resourceFactoryRegistry );

    private:
        std::shared_ptr<EList<std::shared_ptr<EResource>>> initResources();
     
    private:
        Lazy<std::shared_ptr<EList<std::shared_ptr<EResource>>>> resources_;
        Lazy<std::shared_ptr<URIConverter>> uriConverter_;
        Lazy<std::shared_ptr<EResourceFactoryRegistry>> resourceFactoryRegistry_;
    };

}

#endif
