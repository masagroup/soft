#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "ecore/EClass.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EOperation.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EList.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"

using namespace ecore;

namespace std
{
    template <typename T>
    bool operator ==( const std::shared_ptr<EList<T>>& lhs, const std::vector<T>& rhs )
    {
        if (lhs->size() != rhs.size())
            return false;
        for (int i = 0; i < lhs->size(); ++i)
        {
            if (lhs->get( i ) != rhs.at( i ))
                return false;
        }
        return true;
    }

    template <typename T>
    ostream& operator <<( ostream& os, const std::shared_ptr<EList<T>>& v )
    {
        bool first = true;
        os << "[";
        for (auto b : *v)
        {
            os << (first ? "" : ",") << b;
            first = false;
        }
        os << "]";
        return os;
    }

    template <typename T>
    ostream& operator <<( ostream& os, const std::vector<T>& v )
    {
        bool first = true;
        os << "[";
        for (auto b : v)
        {
            os << (first ? "" : ",") << b;
            first = false;
        }
        os << "]";
        return os;
    }

}

BOOST_AUTO_TEST_SUITE( EClassTests )

BOOST_AUTO_TEST_CASE( Constructor )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    BOOST_CHECK( eClass );
}

BOOST_AUTO_TEST_CASE( SuperClass )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    eClass->setName( "MyClass" );

    auto eSuperClass = EcoreFactory::eInstance()->createEClass();
    eSuperClass->setName( "MySuperClass" );

    eClass->getESuperTypes()->add( eSuperClass );
}

BOOST_AUTO_TEST_CASE( StructuralFeatures_Add )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eAttribute = EcoreFactory::eInstance()->createEAttribute();
    BOOST_CHECK_EQUAL( eAttribute->getFeatureID(), -1 );
    eClass->getEStructuralFeatures()->add( eAttribute );
    BOOST_CHECK_EQUAL( eClass->getFeatureCount() , 1);
    BOOST_CHECK_EQUAL( eAttribute->getFeatureID(), 0 );
}

BOOST_AUTO_TEST_CASE( StructuralFeatures_Getters )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eAttribute1 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute2 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute3 = EcoreFactory::eInstance()->createEAttribute();
    auto eReference1 = EcoreFactory::eInstance()->createEReference();
    auto eReference2 = EcoreFactory::eInstance()->createEReference();
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );
    eClass->getEStructuralFeatures()->add( 0, eReference1 );
    eClass->getEStructuralFeatures()->add( 0, eReference2 );

    BOOST_CHECK_EQUAL( eClass->getFeatureCount(), 4 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 0 ), eReference2 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 1 ), eReference1 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 2 ), eAttribute1 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 3 ), eAttribute2 );

    BOOST_CHECK_EQUAL( eAttribute1->getFeatureID(), 2 );
    BOOST_CHECK_EQUAL( eAttribute2->getFeatureID(), 3 );
    BOOST_CHECK_EQUAL( eReference1->getFeatureID(), 1 );
    BOOST_CHECK_EQUAL( eReference2->getFeatureID(), 0 );

    BOOST_CHECK_EQUAL( eClass->getEStructuralFeatures(), std::vector<std::shared_ptr<EStructuralFeature>>( { eReference2, eReference1, eAttribute1, eAttribute2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllStructuralFeatures(), std::vector<std::shared_ptr<EStructuralFeature>>( { eReference2, eReference1, eAttribute1, eAttribute2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAttributes(), std::vector<std::shared_ptr<EAttribute>>( { eAttribute1, eAttribute2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllAttributes(), std::vector<std::shared_ptr<EAttribute>>( { eAttribute1, eAttribute2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEReferences(), std::vector<std::shared_ptr<EReference>>( { eReference2 , eReference1  } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllReferences(), std::vector<std::shared_ptr<EReference>>( { eReference2 , eReference1 } ) );

    eClass->getEStructuralFeatures()->add( eAttribute3 );
    BOOST_CHECK_EQUAL( eClass->getEAttributes(), std::vector<std::shared_ptr<EAttribute>>( { eAttribute1, eAttribute2 , eAttribute3 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllAttributes(), std::vector<std::shared_ptr<EAttribute>>( { eAttribute1, eAttribute2 , eAttribute3 } ) );
}



BOOST_AUTO_TEST_CASE( AttributeID )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eAttribute1 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute2 = EcoreFactory::eInstance()->createEAttribute();
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );
    BOOST_CHECK( !eClass->getEIDAttribute() );

    /*eAttribute1->setID( true );
    BOOST_CHECK_EQUAL( eClass->getEIDAttribute(), eAttribute1 );*/

}

BOOST_AUTO_TEST_CASE( Operations )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eOperation1 = EcoreFactory::eInstance()->createEOperation();
    auto eOperation2 = EcoreFactory::eInstance()->createEOperation();

    BOOST_CHECK_EQUAL( eOperation1->getOperationID(), -1 );
    BOOST_CHECK_EQUAL( eOperation2->getOperationID(), -1 );

    eClass->getEOperations()->add( eOperation1 );
    eClass->getEOperations()->add( eOperation2 );

    BOOST_CHECK_EQUAL( eClass->getOperationCount(), 2 );

    BOOST_CHECK_EQUAL( eOperation1->getOperationID(), 0 );
    BOOST_CHECK_EQUAL( eOperation2->getOperationID(), 1 );

    BOOST_CHECK_EQUAL( eClass->getEOperations(), std::vector<std::shared_ptr<EOperation>>( { eOperation1, eOperation2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllOperations(), std::vector<std::shared_ptr<EOperation>>( { eOperation1, eOperation2 } ) );
}

BOOST_AUTO_TEST_CASE( StructuralFeatures_With_SuperType )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eSuperClass = EcoreFactory::eInstance()->createEClass();
    eClass->getESuperTypes()->add( eSuperClass );

    // add features to tghe class
    auto eAttribute1 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute2 = EcoreFactory::eInstance()->createEAttribute();
    auto eReference1 = EcoreFactory::eInstance()->createEReference();
    auto eReference2 = EcoreFactory::eInstance()->createEReference();
    eClass->getEStructuralFeatures()->add( eAttribute1 );
    eClass->getEStructuralFeatures()->add( eAttribute2 );
    eClass->getEStructuralFeatures()->add( eReference1 );
    eClass->getEStructuralFeatures()->add( eReference2 );

    // test features
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeatures(), std::vector<std::shared_ptr<EStructuralFeature>>( { eAttribute1, eAttribute2, eReference1, eReference2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllStructuralFeatures(), std::vector<std::shared_ptr<EStructuralFeature>>( { eAttribute1, eAttribute2,eReference1, eReference2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 0 ), eAttribute1 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 1 ), eAttribute2 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 2 ), eReference1 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 3 ), eReference2 );

    // add features to the superclass
    auto eAttribute3 = EcoreFactory::eInstance()->createEAttribute();
    auto eAttribute4 = EcoreFactory::eInstance()->createEAttribute();
    auto eReference3 = EcoreFactory::eInstance()->createEReference();
    eSuperClass->getEStructuralFeatures()->add( eAttribute3 );
    eSuperClass->getEStructuralFeatures()->add( eAttribute4 );
    eSuperClass->getEStructuralFeatures()->add( eReference3 );

    // test features
    BOOST_CHECK_EQUAL( eClass->getFeatureCount(), 7 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeatures(), std::vector<std::shared_ptr<EStructuralFeature>>( { eAttribute1, eAttribute2, eReference1, eReference2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllStructuralFeatures(), std::vector<std::shared_ptr<EStructuralFeature>>( { eAttribute3, eAttribute4, eReference3 , eAttribute1,eAttribute2,eReference1,eReference2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 0 ), eAttribute3 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 1 ), eAttribute4 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 2 ), eReference3 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 3 ), eAttribute1 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 4 ), eAttribute2 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 5 ), eReference1 );
    BOOST_CHECK_EQUAL( eClass->getEStructuralFeature( 6 ), eReference2 );

    BOOST_CHECK_EQUAL( eClass->getEAttributes(), std::vector<std::shared_ptr<EAttribute>>( { eAttribute1, eAttribute2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllAttributes(), std::vector<std::shared_ptr<EAttribute>>( { eAttribute3, eAttribute4, eAttribute1,eAttribute2 } ) );

    BOOST_CHECK_EQUAL( eClass->getEReferences(), std::vector<std::shared_ptr<EReference>>( { eReference1, eReference2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllReferences(), std::vector<std::shared_ptr<EReference>>( { eReference3, eReference1, eReference2 } ) );
}

BOOST_AUTO_TEST_CASE( Operations_With_SuperType )
{
    auto eClass = EcoreFactory::eInstance()->createEClass();
    auto eSuperClass = EcoreFactory::eInstance()->createEClass();
    eClass->getESuperTypes()->add( eSuperClass );

    // add operations to the class
    auto eOperation1 = EcoreFactory::eInstance()->createEOperation();
    auto eOperation2 = EcoreFactory::eInstance()->createEOperation();
    eClass->getEOperations()->add( eOperation1 );
    eClass->getEOperations()->add( eOperation2 );

    BOOST_CHECK_EQUAL( eClass->getOperationCount(), 2 );
    BOOST_CHECK_EQUAL( eOperation1->getOperationID(), 0 );
    BOOST_CHECK_EQUAL( eOperation2->getOperationID(), 1 );
    BOOST_CHECK_EQUAL( eClass->getEOperations(), std::vector<std::shared_ptr<EOperation>>( { eOperation1, eOperation2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllOperations(), std::vector<std::shared_ptr<EOperation>>( { eOperation1, eOperation2 } ) );

    // add operations to the superclass
    auto eOperation3 = EcoreFactory::eInstance()->createEOperation();
    auto eOperation4 = EcoreFactory::eInstance()->createEOperation();
    eSuperClass->getEOperations()->add( eOperation3 );
    eSuperClass->getEOperations()->add( eOperation4 );

    // test operations
    BOOST_CHECK_EQUAL( eClass->getOperationCount(), 4 );
    BOOST_CHECK_EQUAL( eClass->getEOperations(), std::vector<std::shared_ptr<EOperation>>( { eOperation1, eOperation2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEAllOperations(), std::vector<std::shared_ptr<EOperation>>( { eOperation3, eOperation4, eOperation1 , eOperation2 } ) );
    BOOST_CHECK_EQUAL( eClass->getEOperation( 0 ), eOperation3 );
    BOOST_CHECK_EQUAL( eClass->getEOperation( 1 ), eOperation4 );
    BOOST_CHECK_EQUAL( eClass->getEOperation( 2 ), eOperation1 );
    BOOST_CHECK_EQUAL( eClass->getEOperation( 3 ), eOperation2 );
}

BOOST_AUTO_TEST_SUITE_END()
