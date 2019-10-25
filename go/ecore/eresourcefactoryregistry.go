package ecore

import "net/url"

//EResourceFactory ...
type EResourceFactoryRegistry interface {
	getFactory(uri *url.URL) EResourceFactory
	getProtocolToFactoryMap() map[string]EResourceFactory
	getExtensionToFactoryMap() map[string]EResourceFactory
}
