package ecore

import "net/url"

const (
	RESOURCE_SET__RESOURCES = 0
)

//EResourceSet ...
type EResourceSet interface {
	createResource(uri *url.URL) EResource
	getResources(uri *url.URL, loadOnDemand bool) EList
	getResource(uri *url.URL, loadOnDemand bool) EResource

	getEObject(uri *url.URL, loadOnDemand bool) EObject

	getURIConverter() EURIConverter
	setURIConverter(uriConverter EURIConverter)

	getResourceFactoryRegistry() EResourceFactoryRegistry
	setResourceFactoryRegistry(resourceFactoryRegistry EResourceFactoryRegistry)

	setURIResourceMap(uriMap map[*url.URL]EResource)
	getURIResourceMap() map[*url.URL]EResource
}
