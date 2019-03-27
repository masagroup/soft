// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_ADAPTER_HPP_
#define ECORE_MOCK_ADAPTER_HPP_

#include "ecore/EAdapter.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    MOCK_BASE_CLASS( MockAdapter , EAdapter )
    {
        MOCK_METHOD( notifyChanged, 1 );
        MOCK_METHOD( getTarget , 0 );
        MOCK_METHOD( setTarget, 1 );
    };
}

#endif
