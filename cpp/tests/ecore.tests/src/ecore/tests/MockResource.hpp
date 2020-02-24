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
    template <typename... I>
    class MockResourceBase : public MockNotifierBase<I...>
    {
    public:
        typedef EResource base_type;

        MOCK_METHOD( getResourceSet, 0 )
        MOCK_METHOD( getURI, 0 )
        MOCK_METHOD( setURI, 1 )
        MOCK_METHOD( getContents, 0 )
        MOCK_METHOD( getAllContents, 0 )
        MOCK_METHOD( getEObject, 1 )
        MOCK_METHOD( getURIFragment, 1 )
        MOCK_METHOD( attached, 1 )
        MOCK_METHOD( detached, 1 )
        MOCK_METHOD_EXT( load, 0, void(), loadSimple )
        MOCK_METHOD_EXT( load, 1, void( std::istream& ), loadFromStream )
        MOCK_METHOD( unload, 0 )
        MOCK_METHOD( isLoaded, 0 )
        MOCK_METHOD_EXT( save, 0, void(), saveSimple )
        MOCK_METHOD_EXT( save, 1, void( std::ostream& ), saveToStream )
        MOCK_METHOD( getErrors, 0 )
        MOCK_METHOD( getWarnings, 0 )
    };

    typedef MockResourceBase<EResource> MockResource;

} // namespace ecore::tests

#endif
