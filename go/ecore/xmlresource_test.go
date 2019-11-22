package ecore

import (
	"net/url"
	"testing"

	"github.com/stretchr/testify/assert"
)

func m(a, b interface{}) []interface{} {
	return []interface{}{a, b}
}

func TestXmlNamespacesNoContext(t *testing.T) {
	n := newXmlNamespaces()
	assert.Equal(t, m("", false), m(n.getURI("prefix")))
	assert.Equal(t, m("", false), m(n.getPrefix("uri")))
}

func TestXmlNamespacesEmpty(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.Equal(t, m("", false), m(n.getURI("prefix")))
	assert.Equal(t, m("", false), m(n.getPrefix("uri")))
	c := n.popContext()
	assert.Equal(t, 0, len(c))
}

func TestXmlNamespacesContext(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri"))
	assert.Equal(t, m("uri", true), m(n.getURI("prefix")))
	assert.Equal(t, m("prefix", true), m(n.getPrefix("uri")))

	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri2"))
	assert.Equal(t, m("uri2", true), m(n.getURI("prefix")))
	assert.Equal(t, m("prefix", true), m(n.getPrefix("uri2")))

	n.popContext()
	assert.Equal(t, m("uri", true), m(n.getURI("prefix")))
	assert.Equal(t, m("prefix", true), m(n.getPrefix("uri")))

	n.popContext()
	assert.Equal(t, m("", false), m(n.getURI("prefix")))
	assert.Equal(t, m("", false), m(n.getPrefix("uri")))
}

func TestXmlNamespacesContextRemap(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri"))
	assert.Equal(t, m("uri", true), m(n.getURI("prefix")))
	assert.Equal(t, m("prefix", true), m(n.getPrefix("uri")))

	assert.True(t, n.declarePrefix("prefix", "uri2"))
	assert.Equal(t, m("uri2", true), m(n.getURI("prefix")))
	assert.Equal(t, m("prefix", true), m(n.getPrefix("uri2")))
}

func TestXmlNamespacesContextNoRemap(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri"))
	assert.Equal(t, m("uri", true), m(n.getURI("prefix")))
	assert.Equal(t, m("prefix", true), m(n.getPrefix("uri")))

	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri2"))
	assert.Equal(t, m("uri2", true), m(n.getURI("prefix")))
	assert.Equal(t, m("prefix", true), m(n.getPrefix("uri2")))
}

func TestXMLResourceLoad(t *testing.T) {
	resource := NewXMLResource()
	resource.SetURI(&url.URL{Path: "testdata/simple.book.ecore"})
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
