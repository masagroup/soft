// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************
#ifndef ECORE_EOBJECT_MOCKOBJECTINTERNAL_HPP
#define ECORE_EOBJECT_MOCKOBJECTINTERNAL_HPP

#include "turtle/mock.hpp"
#include "ecore/impl/EObjectInternal.hpp"

namespace ecore::tests
{

    MOCK_BASE_CLASS(MockObjectInternal, impl::EObjectInternal )
    {
        MOCK_METHOD(eDirectResource, 0);
        MOCK_METHOD(eSetResource, 2);

        MOCK_METHOD(eObjectForFragmentSegment, 1);
        MOCK_METHOD(eURIFragmentSegment, 2);

        MOCK_METHOD(eInverseAdd, 3);
        MOCK_METHOD(eInverseRemove, 3);

        MOCK_METHOD(eProxyURI, 0);
        MOCK_METHOD(eSetProxyURI, 1);
        MOCK_METHOD(eResolveProxy, 1);
    };
}

#endif /* ECORE_EOBJECT_MOCKOBJECTINTERNAL_HPP */
