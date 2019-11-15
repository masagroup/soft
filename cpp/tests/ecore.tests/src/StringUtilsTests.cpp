#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "ecore/Stream.hpp"
#include "ecore/impl/StringUtils.hpp"


using namespace ecore;
using namespace ecore::impl;

namespace std
{
    template <typename T>
    ostream& operator<<(ostream& os, const std::vector<T>& v)
    {
        return print_container(os, v);
    }

} // namespace std

BOOST_AUTO_TEST_SUITE(StringUtilsTests)

BOOST_AUTO_TEST_CASE(Split)
{
    BOOST_CHECK_EQUAL(split("testnospace", " "), std::vector<std::string>({ "testnospace" }));
    BOOST_CHECK_EQUAL(split("test with space", " "), std::vector<std::string>({"test","with","space"}));
}



BOOST_AUTO_TEST_SUITE_END()
