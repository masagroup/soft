// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_NOTIFIER_HPP_
#define ECORE_MOCK_NOTIFIER_HPP_

#include "ecore/ENotifier.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    template <typename... I>
    class MockNotifierBase : public mock::object, public I...
    {
    public:
        typedef ENotifier base_type;

        MOCK_METHOD( eAdapters, 0 , EList<EAdapter*>&() );
        MOCK_METHOD( eDeliver, 0 );
        MOCK_METHOD( eSetDeliver, 1 );
        MOCK_METHOD( eNotify, 1 );
    };

    typedef MockNotifierBase<ENotifier> MockNotifier;
}

#endif
