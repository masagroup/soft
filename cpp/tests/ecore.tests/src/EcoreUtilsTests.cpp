#include <boost/test/auto_unit_test.hpp>

#include "ecore/EcoreUtils.hpp"
#include "ecore/Stream.hpp"
#include "ecore/tests/MockObject.hpp"
#include "ecore/tests/MockObjectInternal.hpp"
#include "ecore/tests/MockResource.hpp"


using namespace ecore;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE(EcoreUtilsTests)

BOOST_AUTO_TEST_CASE(getID)
{

}

BOOST_AUTO_TEST_CASE(getURI)
{
    URI uri("test://file");
    auto mockObject = std::make_shared<MockObject>();
    auto mockInternal = std::make_shared<MockObjectInternal>();

    MOCK_EXPECT(mockObject->eIsProxy).returns(true);
    MOCK_EXPECT(mockObject->getInternalConst).returns(*mockInternal);
    MOCK_EXPECT(mockInternal->eProxyURI).returns(uri);
    BOOST_CHECK_EQUAL(EcoreUtils::getURI(mockObject), uri); 
}


BOOST_AUTO_TEST_SUITE_END()
