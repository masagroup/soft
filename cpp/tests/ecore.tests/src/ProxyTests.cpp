#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore\tests\MockObject.hpp"
#include "ecore\tests\MockObjectInternal.hpp"
#include "ecore\impl\Proxy.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE(ProxyTests)

BOOST_AUTO_TEST_CASE(SetGeT_NoProxy)
{
    auto mockOwner = std::make_shared<MockObject>();
    auto mockObject = std::make_shared<MockObject>();
    Proxy<std::shared_ptr<EObject>> proxy(mockOwner, 1);
    MOCK_EXPECT(mockObject->eIsProxy).returns(false);
    proxy.set(mockObject);
    BOOST_CHECK_EQUAL(proxy.get(), mockObject);
}

BOOST_AUTO_TEST_CASE(SetGeT_WithProxy)
{
    auto mockOwner = std::make_shared<MockObject>();
    auto mockProxy = std::make_shared<MockObject>();
    auto mockResolved = std::make_shared<MockObject>();
    auto mockInternal = std::make_shared<MockObjectInternal>();

    Proxy<std::shared_ptr<EObject>> proxy(mockOwner, 1);
    MOCK_EXPECT(mockProxy->eIsProxy).returns(true);
    proxy.set(mockProxy);

    MOCK_EXPECT(mockOwner->eDeliver).returns(false);
    MOCK_EXPECT(mockOwner->getInternalNonConst).returns(*mockInternal);
    MOCK_EXPECT(mockInternal->eResolveProxy).with(mockProxy).returns(mockResolved);
    BOOST_CHECK_EQUAL(proxy.get(), mockResolved);
}



BOOST_AUTO_TEST_SUITE_END()
