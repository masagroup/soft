#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/tests/MockPackage.hpp"
#include "ecore/tests/MockObjectInternal.hpp"

using namespace ecore;
using namespace ecore::tests;

BOOST_AUTO_TEST_SUITE(EFactoryTests)

BOOST_AUTO_TEST_CASE(Constructor)
{
    auto ecorePackage = EcorePackage::eInstance();
    auto ecoreFactory = EcoreFactory::eInstance();

    auto eFactory = ecoreFactory->createEFactory();
    BOOST_CHECK(eFactory);
}

BOOST_AUTO_TEST_CASE(Accessors_Package)
{
    auto ecorePackage = EcorePackage::eInstance();
    auto ecoreFactory = EcoreFactory::eInstance();

    auto eFactory = ecoreFactory->createEFactory();
    auto mockPackage = std::make_shared<MockPackage>();
    auto mockInternal = std::make_shared<MockObjectInternal>();
    MOCK_EXPECT(mockPackage->getInternalConst).returns(*mockInternal);

    MOCK_EXPECT(mockPackage->eResource).returns(nullptr);
    MOCK_EXPECT(mockInternal->eInverseAdd).with(eFactory, EcorePackage::EFACTORY__EPACKAGE, nullptr).returns(nullptr);
    eFactory->setEPackage(mockPackage);
    BOOST_CHECK_EQUAL(eFactory->getEPackage(), mockPackage);
}



BOOST_AUTO_TEST_SUITE_END()