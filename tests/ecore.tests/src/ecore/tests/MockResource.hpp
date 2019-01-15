// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_RESOURCE_HPP_
#define ECORE_MOCK_RESOURCE_HPP_

#include "ecore/EResource.hpp"
#include "ecore/tests/MockNotifier.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    class MockResource : public virtual MockNotifier, public virtual EResource
    {
    public:
        typedef EResource base_type;

        MOCK_METHOD( getResourceSet, 0 );
        MOCK_METHOD( getUri, 0 );
        MOCK_METHOD( setUri, 1 );
        MOCK_METHOD( getContents, 0 );
        MOCK_METHOD( getAllContents, 0 );
        MOCK_METHOD( attached, 1 );
        MOCK_METHOD( detached, 1 );
    };
}

#endif
