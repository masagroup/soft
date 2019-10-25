package ecore

import (
	"io"
	"net/url"
)

//URIHandler ...
type EURIHandler interface {
	CanHandle(uri *url.URL) bool

	CreateReader(uri *url.URL) io.Reader

	CreateWriter(uri *url.URL) io.Writer
}
