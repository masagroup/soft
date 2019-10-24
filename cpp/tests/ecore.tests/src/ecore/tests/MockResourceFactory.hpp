// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_RESOURCE_FACTORY_HPP_
#define ECORE_MOCK_RESOURCE_FACTORY_HPP_

#include "ecore/EResourceFactory.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    MOCK_BASE_CLASS( MockResourceFactory ,  EResourceFactory )
    {
        MOCK_METHOD(createResource, 1)
    };
}

#endif