package ecore

import "net/url"

type XMIResourceFactory struct {
}

func (f *XMIResourceFactory) CreateResource(uri *url.URL) EResource {
	r := NewXMIResource()
	r.SetURI(uri)
	return r
}
