package ecore

import "net/url"

type EPackageResourceRegistry interface {
	GetResource(p EPackage) EResource
}

type EPackageResourceRegistryImpl struct {
	packageToResources map[EPackage]EResource
}

var packageResourceRegistryInstance *EPackageResourceRegistryImpl

func GetPackageResourceRegistry() EPackageResourceRegistry {
	if packageResourceRegistryInstance == nil {
		packageResourceRegistryInstance = &EPackageResourceRegistryImpl{
			packageToResources: make(map[EPackage]EResource),
		}
	}
	return packageResourceRegistryInstance
}

func (registry *EPackageResourceRegistryImpl) GetResource(p EPackage) EResource {
	if resource, ok := registry.packageToResources[p]; ok {
		return resource
	} else {
		uri, _ := url.Parse(p.GetNsURI())
		resource := newXMLResourceImpl()
		resource.SetURI(uri)
		resource.GetContents().Add(p)
		registry.packageToResources[p] = resource
		return resource
	}
}
