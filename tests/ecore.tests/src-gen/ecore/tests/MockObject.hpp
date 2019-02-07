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
#ifndef ECORE_EOBJECT_MOCKOBJECT_HPP
#define ECORE_EOBJECT_MOCKOBJECT_HPP

#include "ecore/EObject.hpp"
#include "ecore/tests/MockNotifier.hpp"

namespace ecore::tests
{

    class MockObject : public virtual MockNotifier, public virtual EObject 
    {
    public:
        typedef EObject base_type;
        MOCK_METHOD(eClass ,0) 
        MOCK_METHOD(eIsProxy ,0) 
        MOCK_METHOD(eResource ,0) 
        MOCK_METHOD(eContainer ,0) 
        MOCK_METHOD(eContainingFeature ,0) 
        MOCK_METHOD(eContainmentFeature ,0) 
        MOCK_METHOD(eContents ,0) 
        MOCK_METHOD(eAllContents ,0) 
        MOCK_METHOD(eCrossReferences ,0) 
        MOCK_METHOD_EXT(eGet ,1, ecore::Any(const std::shared_ptr<ecore::EStructuralFeature>&), eGet_EStructuralFeature) 
        MOCK_METHOD_EXT(eGet ,2, ecore::Any(const std::shared_ptr<ecore::EStructuralFeature>&,bool), eGet_EStructuralFeature_EBoolean) 
        MOCK_METHOD(eSet ,2) 
        MOCK_METHOD(eIsSet ,1) 
        MOCK_METHOD(eUnset ,1) 
        MOCK_METHOD(eInvoke ,2) 
        
        
        // Start of user code MockObject
        MOCK_METHOD( eSetResource, 2 )
        MOCK_METHOD( eInverseAdd, 3 )
        MOCK_METHOD( eInverseRemove, 3 )
        MOCK_METHOD( eProxyUri, 0 )
        MOCK_METHOD( eSetProxyURI, 1 )
        MOCK_METHOD( eResolveProxy, 1)
        // End of user code
    };
} 

#endif /* ECORE_EOBJECT_MOCKOBJECT_HPP */
