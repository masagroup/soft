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

#include "ecore/EResourceSet.hpp"
#include "ecore/impl/AbstractNotifier.hpp"
#include "ecore/impl/Lazy.hpp"

namespace ecore::impl
{
    
    class ResourceSet : public virtual AbstractNotifier< EResourceSet >
    {
    public:
        ResourceSet();

        virtual ~ResourceSet();

        void setThisPtr( const std::shared_ptr<ResourceSet>& resource );
        std::shared_ptr<ResourceSet> getThisPtr() const;

        virtual std::shared_ptr<EList<std::shared_ptr<EResource>>> getResources() const;
   
        virtual std::shared_ptr<EUriConverter> getUriConverter() const;
        virtual void setUriConverter( const std::shared_ptr<EUriConverter>& uriConverter );

        virtual std::shared_ptr<EResourceFactoryRegistry> gerResourceFactoryRegistry() const;
        virtual void setResourceFactoryRegistry( const std::shared_ptr<EResourceFactoryRegistry>& resourceFactoryRegistry );

    private:
        std::shared_ptr<EList<std::shared_ptr<EResource>>> initResources();
     
    private:
        std::weak_ptr<ResourceSet> thisPtr_;
        Lazy<std::shared_ptr<EList<std::shared_ptr<EResource>>>> resources_;
        Lazy<std::shared_ptr<EUriConverter>> uriConverter_;
        Lazy<std::shared_ptr<EResourceFactoryRegistry>> resourceFactoryRegistry_;
    };

}

#endif
