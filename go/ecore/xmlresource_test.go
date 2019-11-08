package ecore

import (
	"net/url"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestXmlNamespacesNoContext(t *testing.T) {
	n := newXmlNamespaces()
	assert.Equal(t, "", n.getURI("prefix"))
	assert.Equal(t, "", n.getPrefix("uri"))
}

func TestXmlNamespacesEmpty(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.Equal(t, "", n.getURI("prefix"))
	assert.Equal(t, "", n.getPrefix("uri"))
	c := n.popContext()
	assert.Equal(t, 0, len(c))
}

func TestXmlNamespacesContext(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri"))
	assert.Equal(t, "uri", n.getURI("prefix"))
	assert.Equal(t, "prefix", n.getPrefix("uri"))

	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri2"))
	assert.Equal(t, "uri2", n.getURI("prefix"))
	assert.Equal(t, "prefix", n.getPrefix("uri2"))

	n.popContext()
	assert.Equal(t, "uri", n.getURI("prefix"))
	assert.Equal(t, "prefix", n.getPrefix("uri"))

	n.popContext()
	assert.Equal(t, "", n.getURI("prefix"))
	assert.Equal(t, "", n.getPrefix("uri"))
}

func TestXmlNamespacesContextRemap(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri"))
	assert.Equal(t, "uri", n.getURI("prefix"))
	assert.Equal(t, "prefix", n.getPrefix("uri"))

	assert.True(t, n.declarePrefix("prefix", "uri2"))
	assert.Equal(t, "uri2", n.getURI("prefix"))
	assert.Equal(t, "prefix", n.getPrefix("uri2"))
}

func TestXmlNamespacesContextNoRemap(t *testing.T) {
	n := newXmlNamespaces()
	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri"))
	assert.Equal(t, "uri", n.getURI("prefix"))
	assert.Equal(t, "prefix", n.getPrefix("uri"))

	n.pushContext()
	assert.False(t, n.declarePrefix("prefix", "uri2"))
	assert.Equal(t, "uri2", n.getURI("prefix"))
	assert.Equal(t, "prefix", n.getPrefix("uri2"))
}

func TestXMLResourceLoad(t *testing.T) {
	resource := NewXMLResource()
	resource.SetURI(&url.URL{Path: "testdata/simple.book.ecore"})
	resource.Load()
	assert.True(t, resource.IsLoaded())
	assert.True(t, resource.GetErrors().Empty())
	assert.True(t, resource.GetWarnings().Empty())
}
