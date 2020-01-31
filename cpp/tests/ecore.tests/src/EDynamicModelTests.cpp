#include <boost/test/unit_test.hpp>

#include "ecore/Constants.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EList.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EcoreUtils.hpp"
#include "ecore/Stream.hpp"
#include "ecore/impl/AbstractResource.hpp"

using namespace ecore;
using namespace ecore::impl;

/*
    Inspired from Example of Dynamic Model : https://www.ibm.com/developerworks/library/os-eclipse-dynamicemf/
*/

namespace
{
    class BookStoreModelFixture
    {
    public:

        BookStoreModelFixture()
        {
            auto ecoreFactory = EcoreFactory::eInstance();
            auto ecorePackage = EcorePackage::eInstance();
            /*
            * Create EClass instance to model BookStore class
            */
            bookStoreEClass = ecoreFactory->createEClass();

            /*
            * Create EClass instance to model Book class
            */
            bookEClass = ecoreFactory->createEClass();

            /*
            * Instantiate EPackage and provide unique URI
            * to identify this package
            */
            bookStoreEFactory = ecoreFactory->createEFactory();

            bookStoreEPackage = ecoreFactory->createEPackage();
            bookStoreEPackage->setName( "BookStorePackage" );
            bookStoreEPackage->setNsPrefix( "bookStore" );
            bookStoreEPackage->setNsURI( "http:///com.ibm.dynamic.example.bookstore.ecore" );
            bookStoreEPackage->setEFactoryInstance( bookStoreEFactory );

            /*
            * Create attributes for BookStore class as specified in the model
            */
            bookStoreOwner = ecoreFactory->createEAttribute();
            bookStoreOwner->setName( "owner" );
            bookStoreOwner->setEType( ecorePackage->getEString() );

            bookStoreLocation = ecoreFactory->createEAttribute();
            bookStoreLocation->setName( "location" );
            bookStoreLocation->setEType( ecorePackage->getEString() );

            bookStore_Books = ecoreFactory->createEReference();
            bookStore_Books->setName( "books" );
            bookStore_Books->setEType( bookEClass );
            bookStore_Books->setUpperBound( UNBOUNDED_MULTIPLICITY );
            bookStore_Books->setContainment( true );

            /*
            * Create attributes for Book class as defined in the model
            */
            bookName = ecoreFactory->createEAttribute();
            bookName->setName( "name" );
            bookName->setEType( ecorePackage->getEString() );

            bookISBN = ecoreFactory->createEAttribute();
            bookISBN->setName( "isbn" );
            bookISBN->setEType( ecorePackage->getEInt() );

            /*
            * Add owner, location and books attributes/references
            * to BookStore class
            */
            bookStoreEClass->getEStructuralFeatures()->add( bookStoreOwner );
            bookStoreEClass->getEStructuralFeatures()->add( bookStoreLocation );
            bookStoreEClass->getEStructuralFeatures()->add( bookStore_Books );

            /*
            * Add name and isbn attributes to Book class
            */
            bookEClass->getEStructuralFeatures()->add( bookName );
            bookEClass->getEStructuralFeatures()->add( bookISBN );

            /*
            * Place BookStore and Book classes in bookStoreEPackage
            */
            bookStoreEPackage->getEClassifiers()->add( bookStoreEClass );
            bookStoreEPackage->getEClassifiers()->add( bookEClass );

        }

    protected:
        std::shared_ptr<EPackage> bookStoreEPackage;
        std::shared_ptr<EFactory> bookStoreEFactory;

        std::shared_ptr<EClass> bookStoreEClass;
        std::shared_ptr<EAttribute> bookStoreOwner;
        std::shared_ptr<EAttribute> bookStoreLocation;
        std::shared_ptr<EReference> bookStore_Books;
        
        std::shared_ptr<EClass> bookEClass;
        std::shared_ptr<EAttribute> bookName;
        std::shared_ptr<EAttribute> bookISBN;
    };

    class BookStoreInstanciateModel : public BookStoreModelFixture {
    public:
        BookStoreInstanciateModel()
            : BookStoreModelFixture() {
            /*
             * Obtain EFactory instance from BookStoreEPackage
             */
            auto bookFactoryInstance = bookStoreEPackage->getEFactoryInstance();

            /*
             * Create dynamic instance of BookStoreEClass and BookEClass
             */
            bookObject = bookFactoryInstance->create(bookEClass);
            bookStoreObject = bookFactoryInstance->create(bookStoreEClass);

            /*
            * Set the values of bookStoreObject attributes
            */
            bookStoreObject->eSet(bookStoreOwner, std::string("David Brown"));
            bookStoreObject->eSet(bookStoreLocation, std::string("Street#12, Top Town, NY"));
            auto anyBooks = bookStoreObject->eGet(bookStore_Books);
            auto allBooks = anyCast<std::shared_ptr<EList< std::shared_ptr<EObject>>>>(anyBooks);
            allBooks->add(bookObject);

            /*
            * Set the values of bookObject attributes
            */
            bookObject->eSet(bookName, std::string("Harry Potter and the Deathly Hallows"));
            bookObject->eSet(bookISBN, 157221);
        }

    protected:
        std::shared_ptr<EObject> bookObject;
        std::shared_ptr<EObject> bookStoreObject;
    };

    class Resource : public AbstractResource
    {
    public:
        Resource()
            : AbstractResource() {
        }

        Resource(const URI& uri)
            : AbstractResource(uri) {
        }
    private:
        virtual void doLoad(std::istream& is) override {
        }

        virtual void doSave(std::ostream& os) override {
        }
    };
}

BOOST_AUTO_TEST_SUITE( EDynamicModelTests )


BOOST_FIXTURE_TEST_CASE( InstanciateModel , BookStoreInstanciateModel)
{
   /*
    * Read/Get the values of bookStoreObject attributes
    */
    auto anyOwner = bookStoreObject->eGet( bookStoreOwner );
    auto strOwner = anyCast<std::string>( anyOwner );
    BOOST_CHECK_EQUAL(strOwner, "David Brown");

    auto anyLocation = bookStoreObject->eGet( bookStoreLocation );
    auto strLocation = anyCast<std::string>( anyLocation );
    BOOST_CHECK_EQUAL(strLocation, "Street#12, Top Town, NY");

    /*
    * Read/Get the values of bookObject attributes
    */
    auto anyName = bookObject->eGet( bookName );
    auto strName = anyCast<std::string>( anyName );
    BOOST_CHECK_EQUAL(strName, "Harry Potter and the Deathly Hallows");

    auto anyISBN = bookObject->eGet( bookISBN );
    auto intISBN = anyCast<int>( anyISBN );
    BOOST_CHECK_EQUAL(intISBN, 157221);   
}

BOOST_FIXTURE_TEST_CASE(getURI_NoResource, BookStoreInstanciateModel)
{
    BOOST_CHECK_EQUAL(EcoreUtils::getURI(bookStoreObject), URI("#//"));
    BOOST_CHECK_EQUAL(EcoreUtils::getURI(bookObject), URI("#//@books.0"));
}

BOOST_FIXTURE_TEST_CASE(getURI_Resource, BookStoreInstanciateModel)
{
    auto resource = std::make_shared<Resource>( URI("file://a.test") );
    resource->setThisPtr(resource);

    auto contents = resource->getContents();
    contents->add(bookStoreObject);

    BOOST_CHECK_EQUAL(EcoreUtils::getURI(bookStoreObject), URI("file://a.test#/"));
    BOOST_CHECK_EQUAL(EcoreUtils::getURI(bookObject), URI("file://a.test#//@books.0"));
}

BOOST_AUTO_TEST_SUITE_END()
