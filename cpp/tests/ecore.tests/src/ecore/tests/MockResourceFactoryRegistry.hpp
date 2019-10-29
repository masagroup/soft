// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_RESOURCE_FACTORY_REGISTRY_HPP_
#define ECORE_MOCK_RESOURCE_FACTORY_REGISTRY_HPP_

#include "ecore/EResourceFactoryRegistry.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    MOCK_BASE_CLASS(MockResourceFactoryRegistry, EResourceFactoryRegistry)
    {
        MOCK_METHOD(getFactory, 1)
        MOCK_METHOD(getProtocolToFactoryMap, 0)
        MOCK_METHOD(getExtensionToFactoryMap, 0)
    };
}

#endif