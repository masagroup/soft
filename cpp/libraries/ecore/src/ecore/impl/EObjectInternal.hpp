// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

// *****************************************************************************
//    
// Warning: This file was generated by soft.generator.cpp Generator
// 
//*****************************************************************************
#ifndef ECORE_EOBJECTINTERNAL_HPP
#define ECORE_EOBJECTINTERNAL_HPP

#include "ecore/Exports.hpp"
#include "ecore/EObject.hpp"

namespace ecore::impl
{
    class ECORE_API EObjectInternal : public virtual EObject
    {
    public:
        virtual ~EObjectInternal() = default;

        virtual std::shared_ptr<EResource> eDirectResource() const = 0;

        virtual std::shared_ptr<EObject> eObjectForFragmentSegment( const std::string& uriSegment) const= 0;
        
        virtual std::string eURIFragmentSegment(const std::shared_ptr<EStructuralFeature>& feature, const std::shared_ptr<EObject>& eObject) const = 0;

    };
}

#endif /* ECORE_EOBJECTINTERNAL_HPP */
