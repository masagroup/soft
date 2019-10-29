package ecore

import (
	"net/url"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestXMLResourceLoad(t *testing.T) {
	resource := NewXMLResource()
	resource.SetURI(&url.URL{Path: "testdata/simple.book.ecore"})
	resource.Load()
	assert.True(t, resource.IsLoaded())
}
