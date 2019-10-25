package ecore

import (
	"io"
	"net/url"
)

//URIHandler ...
type EURIHandler interface {
	createInputStream(uri *url.URL) io.Reader

	createOutputStream(uri *url.URL) io.Writer
}
