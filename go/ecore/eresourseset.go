package ecore

import "net/url"

const (
	RESOURCE_SET__RESOURCES = 0
)

//EResourceSet ...
type EResourceSet interface {
	GetResources() EList
	GetResource(uri *url.URL, loadOnDemand bool) EResource
	CreateResource(uri *url.URL) EResource

	GetEObject(uri *url.URL, loadOnDemand bool) EObject

	GetURIConverter() EURIConverter
	SetURIConverter(uriConverter EURIConverter)

	GetResourceFactoryRegistry() EResourceFactoryRegistry
	SetResourceFactoryRegistry(resourceFactoryRegistry EResourceFactoryRegistry)

	SetURIResourceMap(uriMap map[*url.URL]EResource)
	GetURIResourceMap() map[*url.URL]EResource
}
