// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

import (
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestDynamicModel(t *testing.T) {
	/*
	* Create EClass instance to model BookStore class
	 */
	bookStoreEClass := GetFactory().CreateEClass()

	/*
	* Create EClass instance to model Book class
	 */
	bookEClass := GetFactory().CreateEClass()

	/*
	* Instantiate EPackage and provide unique URI
	* to identify this package
	 */
	bookStoreEFactory := GetFactory().CreateEFactory()

	bookStoreEPackage := GetFactory().CreateEPackage()
	bookStoreEPackage.SetName("BookStorePackage")
	bookStoreEPackage.SetNsPrefix("bookStore")
	bookStoreEPackage.SetNsURI("http:///com.ibm.dynamic.example.bookstore.ecore")
	bookStoreEPackage.SetEFactoryInstance(bookStoreEFactory)

	/*
	* Create attributes for BookStore class as specified in the model
	 */
	bookStoreOwner := GetFactory().CreateEAttribute()
	bookStoreOwner.SetName("owner")
	bookStoreOwner.SetEType(GetPackage().GetEString())

	bookStoreLocation := GetFactory().CreateEAttribute()
	bookStoreLocation.SetName("location")
	bookStoreLocation.SetEType(GetPackage().GetEString())

	bookStoreBooks := GetFactory().CreateEReference()
	bookStoreBooks.SetName("books")
	bookStoreBooks.SetEType(bookEClass)
	bookStoreBooks.SetUpperBound(UNBOUNDED_MULTIPLICITY)
	bookStoreBooks.SetContainment(true)

	/*
	* Create attributes for Book class as defined in the model
	 */
	bookName := GetFactory().CreateEAttribute()
	bookName.SetName("name")
	bookName.SetEType(GetPackage().GetEString())

	bookISBN := GetFactory().CreateEAttribute()
	bookISBN.SetName("isbn")
	bookISBN.SetEType(GetPackage().GetEInt())

	/*
	* Add owner, location and books attributes/references
	* to BookStore class
	 */
	bookStoreEClass.GetEStructuralFeatures().Add(bookStoreOwner)
	bookStoreEClass.GetEStructuralFeatures().Add(bookStoreLocation)
	bookStoreEClass.GetEStructuralFeatures().Add(bookStoreBooks)

	/*
	* Add name and isbn attributes to Book class
	 */
	bookEClass.GetEStructuralFeatures().Add(bookName)
	bookEClass.GetEStructuralFeatures().Add(bookISBN)

	/*
	* Place BookStore and Book classes in bookStoreEPackage
	 */
	bookStoreEPackage.GetEClassifiers().Add(bookStoreEClass)
	bookStoreEPackage.GetEClassifiers().Add(bookEClass)

	/*
	* Instanticate model
	 */

	/*
	 * Obtain EFactory instance from BookStoreEPackage
	 */
	bookFactoryInstance := bookStoreEPackage.GetEFactoryInstance()

	/*
	 * Create dynamic instance of BookStoreEClass and BookEClass
	 */
	bookObject := bookFactoryInstance.Create(bookEClass)
	bookStoreObject := bookFactoryInstance.Create(bookStoreEClass)

	/*
	 * Set the values of bookStoreObject attributes
	 */
	bookStoreObject.ESet(bookStoreOwner, "David Brown")
	bookStoreObject.ESet(bookStoreLocation, "Street#12, Top Town, NY")
	allBooks := bookStoreObject.EGet(bookStoreBooks).(EList)
	allBooks.Add(bookObject)

	/*
	 * Set the values of bookObject attributes
	 */
	bookObject.ESet(bookName, "Harry Potter and the Deathly Hallows")
	bookObject.ESet(bookISBN, 157221)

	/*
	 * Read/Get the values of bookStoreObject attributes
	 */
	assert.Equal(t, "David Brown", bookStoreObject.EGet(bookStoreOwner).(string))
	assert.Equal(t, "Street#12, Top Town, NY", bookStoreObject.EGet(bookStoreLocation).(string))

	/*
	 * Read/Get the values of bookObject attributes
	 */
	assert.Equal(t, "Harry Potter and the Deathly Hallows", bookObject.EGet(bookName).(string))
	assert.Equal(t, 157221, bookObject.EGet(bookISBN).(int))

}
