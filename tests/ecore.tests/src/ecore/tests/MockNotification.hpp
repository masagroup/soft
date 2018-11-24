// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_MOCK_NOTIFICATION_HPP_
#define ECORE_MOCK_NOTIFICATION_HPP_

#include "ecore/ENotification.hpp"
#include "turtle/mock.hpp"

namespace ecore::tests
{
    MOCK_BASE_CLASS( MockNotification , ENotification )
    {
        MOCK_METHOD( getEventType, 0 );
        MOCK_METHOD( getNotifier, 0 );
        MOCK_METHOD( getFeature, 0 );
        MOCK_METHOD( getFeatureID, 0 );
        MOCK_METHOD( getOldValue, 0 );
        MOCK_METHOD( getNewValue, 0 );
        MOCK_METHOD( getPosition, 0 );
        MOCK_METHOD( merge, 1 );
    };
}

#endif // ECORE_MOCK_NOTIFICATION_HPP_
