package ecore

import (
	"io"
	"net/url"
)

//URIConverter ...
type EURIConverter interface {
	createInputStream(uri *url.URL) io.Reader

	createOutputStream(uri *url.URL) io.Writer

	normalize(uri *url.URL) *url.URL

	getURIHandler(uri *url.URL) EURIHandler

	getURIHandlers() EList
}
