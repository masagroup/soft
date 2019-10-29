package ecore

import "net/url"

//EResourceFactory ...
type EResourceFactory interface {
	CreateResource(uri *url.URL) EResource
}
