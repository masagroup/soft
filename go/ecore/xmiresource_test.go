package ecore

import (
	"io/ioutil"
	"net/url"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestXMIResourceLoadSimple(t *testing.T) {
	resource := newXMIResourceImpl()
	resource.SetURI(&url.URL{Path: "testdata/bookStore.ecore"})
	resource.Load()
	assert.True(t, resource.IsLoaded())
	assert.True(t, resource.GetErrors().Empty())
	assert.True(t, resource.GetWarnings().Empty())

	contents := resource.GetContents()
	assert.Equal(t, 1, contents.Size())

	ePackage, _ := contents.Get(0).(EPackage)
	assert.NotNil(t, ePackage)
	assert.Equal(t, "BookStorePackage", ePackage.GetName())
	assert.Equal(t, "bookStore", ePackage.GetNsPrefix())
	assert.Equal(t, "http:///com.ibm.dynamic.example.bookStore.ecore", ePackage.GetNsURI())

	eClassifiers := ePackage.GetEClassifiers()
	assert.Equal(t, 2, eClassifiers.Size())

	eBookStore, _ := eClassifiers.Get(0).(EClassifier)
	assert.NotNil(t, eBookStore)
	assert.Equal(t, "BookStore", eBookStore.GetName())

	eBookStoreClass, _ := eBookStore.(EClass)
	assert.NotNil(t, eBookStoreClass)
	assert.Equal(t, 3, eBookStoreClass.GetFeatureCount())

	eOwnerFeature := eBookStoreClass.GetEStructuralFeature(0)
	assert.Equal(t, "owner", eOwnerFeature.GetName())
	eOwnerAttribute, _ := eOwnerFeature.(EAttribute)
	assert.NotNil(t, eOwnerAttribute)
	eOwnerAttributeType := eOwnerAttribute.GetEAttributeType()
	assert.Equal(t, "EString", eOwnerAttributeType.GetName())

	eLocationFeature := eBookStoreClass.GetEStructuralFeature(1)
	assert.Equal(t, "location", eLocationFeature.GetName())
	eLocationAttribute, _ := eLocationFeature.(EAttribute)
	assert.NotNil(t, eLocationAttribute)
	eLocationType := eLocationAttribute.GetEAttributeType()
	assert.NotNil(t, eLocationType)

	eBooksFeature := eBookStoreClass.GetEStructuralFeature(2)
	assert.Equal(t, "books", eBooksFeature.GetName())
	eBooksReference, _ := eBooksFeature.(EReference)
	assert.NotNil(t, eBooksReference)

	eBook := eClassifiers.Get(1).(EClassifier)
	assert.NotNil(t, eBook)
	assert.Equal(t, "Book", eBook.GetName())
	eBookClass, _ := eBook.(EClass)
	assert.NotNil(t, eBookClass)
	assert.Equal(t, 2, eBookClass.GetFeatureCount())

	eNameFeature := eBookClass.GetEStructuralFeature(0)
	assert.Equal(t, "name", eNameFeature.GetName())
	eNameAttribute, _ := eNameFeature.(EAttribute)
	assert.NotNil(t, eNameAttribute)

	eISBNFeature := eBookClass.GetEStructuralFeature(1)
	assert.Equal(t, "isbn", eISBNFeature.GetName())
	eISBNAttribute, _ := eISBNFeature.(EAttribute)
	assert.NotNil(t, eISBNAttribute)

	// check resolved reference
	assert.Equal(t, eBookClass, eBooksReference.GetEReferenceType())
}

func TestXMIResourceLoadComplex(t *testing.T) {
	resource := newXMIResourceImpl()
	resource.SetURI(&url.URL{Path: "testdata/library.ecore"})
	resource.Load()
	assert.True(t, resource.IsLoaded())
	assert.True(t, resource.GetErrors().Empty())
	assert.True(t, resource.GetWarnings().Empty())

	contents := resource.GetContents()
	assert.Equal(t, 1, contents.Size())

	ePackage, _ := contents.Get(0).(EPackage)
	assert.NotNil(t, ePackage)

	eClassifiers := ePackage.GetEClassifiers()
	eBook, _ := eClassifiers.Get(0).(EClassifier)
	assert.NotNil(t, eBook)
	assert.Equal(t, "Book", eBook.GetName())

	eBookClass, _ := eBook.(EClass)
	assert.NotNil(t, eBookClass)
	superTypes := eBookClass.GetESuperTypes()
	assert.Equal(t, 1, superTypes.Size())
	eCirculationItemClass := superTypes.Get(0).(EClass)
	assert.Equal(t, "CirculatingItem", eCirculationItemClass.GetName())
}

func TestXMIResourceSave(t *testing.T) {

	resource := newXMIResourceImpl()
	resource.SetURI(&url.URL{Path: "testdata/bookStore.ecore"})
	resource.Load()

	var strbuff strings.Builder
	resource.SaveWithWriter(&strbuff)

	bytes, err := ioutil.ReadFile("testdata/bookStore.ecore")
	assert.Nil(t, err)
	assert.Equal(t, strings.ReplaceAll(string(bytes), "\r\n", "\n"), strings.ReplaceAll(strbuff.String(), "\r\n", "\n"))
}

func BenchmarkXMIResourceLoadSaveSimple(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		resource := newXMIResourceImpl()
		resource.SetURI(&url.URL{Path: "testdata/bookStore.ecore"})
		resource.Load()

		var strbuff strings.Builder
		resource.SaveWithWriter(&strbuff)
		resource = nil
	}
}

func BenchmarkXMIResourceLoadComplex(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		resource := newXMIResourceImpl()
		resource.SetURI(&url.URL{Path: "testdata/library.ecore"})
		resource.Load()
	}
}
