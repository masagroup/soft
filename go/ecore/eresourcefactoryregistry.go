package ecore

import "net/url"

const (
	DEFAULT_EXTENSION = "*"
)

//EResourceFactoryRegistry ...
type EResourceFactoryRegistry interface {
	GetFactory(uri *url.URL) EResourceFactory
	GetProtocolToFactoryMap() map[string]EResourceFactory
	GetExtensionToFactoryMap() map[string]EResourceFactory
}

var resourceFactoryRegistryInstance EResourceFactoryRegistry

func GetResourceFactoryRegistry() EResourceFactoryRegistry {
	if resourceFactoryRegistryInstance == nil {
		resourceFactoryRegistryInstance = NewEResourceFactoryRegistryImpl()
	}
	return resourceFactoryRegistryInstance
}
