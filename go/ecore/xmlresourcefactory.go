package ecore

import "net/url"

type XMLResourceFactory struct {
}

func (f *XMLResourceFactory) CreateResource(uri *url.URL) EResource {
	r := NewXMLResource()
	r.SetURI(uri)
	return r
}
