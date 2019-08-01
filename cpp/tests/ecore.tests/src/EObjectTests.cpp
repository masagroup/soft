#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EAdapter.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EOperation.hpp"
#include "ecore/Stream.hpp"
#include "ecore/impl/ImmutableEList.hpp"

using namespace ecore;
using namespace ecore::impl;

namespace std
{
    template <typename T>
    bool operator==( const std::shared_ptr<EList<T>>& lhs, const std::vector<T>& rhs )
    {
        return lhs->size() == rhs.size() && std::equal( lhs->begin(), lhs->end(), rhs.begin() );
    }

    template <typename T>
    bool operator==( const std::shared_ptr<const EList<T>>& lhs, const std::vector<T>& rhs )
    {
        return lhs->size() == rhs.size() && std::equal( lhs->begin(), lhs->end(), rhs.begin() );
    }

    template <typename T>
    ostream& operator<<( ostream& os, const std::shared_ptr<EList<T>>& v )
    {
        return print_container( os, *v );
    }

} // namespace std

BOOST_AUTO_TEST_SUITE( EObjectTests )

BOOST_AUTO_TEST_CASE( Contents )
{
    auto f = EcoreFactory::eInstance();
    auto c = f->createEClass();
    auto a1 = f->createEAttribute();
    auto a2 = f->createEAttribute();
    auto o1 = f->createEOperation();
    c->getEStructuralFeatures()->add(a1);
    c->getEStructuralFeatures()->add(a2);
    c->getEOperations()->add( o1 );
    BOOST_CHECK_EQUAL( c->eContents(), std::vector<std::shared_ptr<EObject>>( {a1, a2, o1} ) );
}

BOOST_AUTO_TEST_SUITE_END()