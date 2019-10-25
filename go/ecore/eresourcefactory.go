package ecore

import "net/url"

//EResourceFactory ...
type EResourceFactory interface {
	createResource(uri *url.URL) EResource
}
