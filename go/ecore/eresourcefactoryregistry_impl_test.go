package ecore

import (
	"net/url"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEResourceFactoryRegistryGetFactoryProtocol(t *testing.T) {
	mockFactory := new(MockEResourceFactory)

	rfr := NewEResourceFactoryRegistryImpl()
	rfr.GetProtocolToFactoryMap()["test"] = mockFactory

	assert.Equal(t, mockFactory, rfr.GetFactory(&url.URL{Scheme: "test", Path: "//file.t"}))
	assert.Nil(t, rfr.GetFactory(&url.URL{Scheme: "file", Path: "//file.t"}))
}

func TestEResourceFactoryRegistryGetFactoryExtension(t *testing.T) {
	mockFactory := new(MockEResourceFactory)

	rfr := NewEResourceFactoryRegistryImpl()
	rfr.GetExtensionToFactoryMap()["test"] = mockFactory

	assert.Equal(t, mockFactory, rfr.GetFactory(&url.URL{Scheme: "file", Path: "//file.test"}))
	assert.Nil(t, rfr.GetFactory(&url.URL{Scheme: "file", Path: "//file.t"}))
}
