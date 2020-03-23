package library

import (
	"net/url"
	"testing"

	"github.com/masagroup/soft/go/ecore"
	"github.com/stretchr/testify/assert"
)

func TestSerializationXML(t *testing.T) {
	ecore.GetPackageRegistry().RegisterPackage(GetPackage())

	fileURI := &url.URL{Path: "testdata/library.xml"}
	resourceFactory := ecore.GetResourceFactoryRegistry().GetFactory(fileURI)
	resource := resourceFactory.CreateResource(fileURI)
	resource.Load()
	assert.True(t, resource.IsLoaded())
	assert.True(t, resource.GetErrors().Empty())
	assert.True(t, resource.GetWarnings().Empty())
}
