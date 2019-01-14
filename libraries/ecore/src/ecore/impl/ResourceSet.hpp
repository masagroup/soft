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

#include "ecore/EResource.hpp"
#include "ecore/impl/Lazy.hpp"

namespace ecore::impl
{
    
    class ResourceSet
    {
    public:
        ResourceSet();

        virtual ~ResourceSet();

        virtual std::shared_ptr<EList<std::shared_ptr<EResource>>> getResources() const;
   
    private:
        virtual std::shared_ptr<EList<std::shared_ptr<EResource>>> initResources();

    private:
        Lazy<std::shared_ptr<EList<std::shared_ptr<EResource>>>> resources_;

    };

}

#endif
