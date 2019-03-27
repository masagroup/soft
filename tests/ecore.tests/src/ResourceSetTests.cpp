#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/impl/AbstractResource.hpp"
#include "ecore/impl/ResourceSet.hpp"
#include "ecore/tests/MockResource.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

namespace
{
    class Resource : public AbstractResource
    {
        // Inherited via AbstractResource
        virtual void doLoad( std::istream& is ) override
        {
            throw std::exception( "NotImplementedException " );
        }
        virtual void doSave( std::ostream& os ) override
        {
            throw std::exception( "NotImplementedException " );
        }
    };
}

BOOST_AUTO_TEST_SUITE( ResourceSetTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );
}

BOOST_AUTO_TEST_CASE( Resources_WithMock )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );

    auto resource = std::make_shared<MockResource>();
    auto resources = resourceSet->getResources();
    resources->add( resource );
}

BOOST_AUTO_TEST_CASE( Resources_NoMock )
{
    auto resourceSet = std::make_shared<ResourceSet>();
    resourceSet->setThisPtr( resourceSet );

    auto resource = std::make_shared<Resource>();
    auto resources = resourceSet->getResources();
    resources->add( resource );
    BOOST_CHECK_EQUAL( resource->getResourceSet(), resourceSet );

    resources->remove( resource );
    BOOST_CHECK_EQUAL( resource->getResourceSet(), nullptr );
}


BOOST_AUTO_TEST_SUITE_END()