package ecore

import "net/url"

const (
	RESOURCE_SET__RESOURCES = 0
)

//EResourceSet ...
type EResourceSet interface {
	CreateResource(uri *url.URL) EResource
	GetResources(uri *url.URL, loadOnDemand bool) EList
	GetResource(uri *url.URL, loadOnDemand bool) EResource

	GetEObject(uri *url.URL, loadOnDemand bool) EObject

	GetURIConverter() EURIConverter
	SetURIConverter(uriConverter EURIConverter)

	GetResourceFactoryRegistry() EResourceFactoryRegistry
	SetResourceFactoryRegistry(resourceFactoryRegistry EResourceFactoryRegistry)

	SetURIResourceMap(uriMap map[*url.URL]EResource)
	GetURIResourceMap() map[*url.URL]EResource
}
