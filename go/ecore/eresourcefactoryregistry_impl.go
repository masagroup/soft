package ecore

import (
	"net/url"
	"strings"
)

//EResourceFactoryRegistryImpl ...
type EResourceFactoryRegistryImpl struct {
	protocolToFactory  map[string]EResourceFactory
	extensionToFactory map[string]EResourceFactory
}

func NewEResourceFactoryRegistryImpl() *EResourceFactoryRegistryImpl {
	r := new(EResourceFactoryRegistryImpl)
	r.protocolToFactory = make(map[string]EResourceFactory)
	r.extensionToFactory = make(map[string]EResourceFactory)
	return r
}

func (r *EResourceFactoryRegistryImpl) GetFactory(uri *url.URL) EResourceFactory {
	if factory, ok := r.protocolToFactory[uri.Scheme]; ok {
		return factory
	}

	ndx := strings.LastIndex(uri.Path, ".")
	if ndx != -1 {
		extension := uri.Path[ndx+1:]
		if factory, ok := r.extensionToFactory[extension]; ok {
			return factory
		}
	}
	if factory, ok := r.extensionToFactory[DEFAULT_EXTENSION]; ok {
		return factory
	}
	return nil
}

func (r *EResourceFactoryRegistryImpl) GetProtocolToFactoryMap() map[string]EResourceFactory {
	return r.protocolToFactory
}

func (r *EResourceFactoryRegistryImpl) GetExtensionToFactoryMap() map[string]EResourceFactory {
	return r.extensionToFactory
}
