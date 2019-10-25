package ecore

import (
	"io"
	"net/url"
)

//URIHandler ...
type URIHandler interface {
	createInputStream(uri *url.URL) io.Reader

	createOutputStream(uri *url.URL) io.Writer
}
