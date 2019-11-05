package ecore

import "net/url"

const (
	RESOURCE_SET__RESOURCES = 0
)

//EResourceSet ...
type EResourceSet interface {
	ENotifier

	GetResources() EList
	GetResource(uri *url.URL, loadOnDemand bool) EResource
	CreateResource(uri *url.URL) EResource

	GetEObject(uri *url.URL, loadOnDemand bool) EObject

	GetURIConverter() EURIConverter
	SetURIConverter(uriConverter EURIConverter)

	GetPackageRegistry() EPackageRegistry
	SetPackageRegistry(packageregistry EPackageRegistry)

	GetResourceFactoryRegistry() EResourceFactoryRegistry
	SetResourceFactoryRegistry(resourceFactoryRegistry EResourceFactoryRegistry)

	SetURIResourceMap(uriMap map[*url.URL]EResource)
	GetURIResourceMap() map[*url.URL]EResource
}
