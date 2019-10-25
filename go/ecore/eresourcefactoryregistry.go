package ecore

import "net/url"

//EResourceFactory ...
type EResourceFactoryRegistry interface {
	GetFactory(uri *url.URL) EResourceFactory
	GetProtocolToFactoryMap() map[string]EResourceFactory
	GetExtensionToFactoryMap() map[string]EResourceFactory
}
