package ecore

import (
	"io"
	"net/url"
)

//URIConverter ...
type EURIConverter interface {
	CreateReader(uri *url.URL) io.Reader

	CreateWriter(uri *url.URL) io.Writer

	Normalize(uri *url.URL) *url.URL

	GetURIHandler(uri *url.URL) EURIHandler

	GetURIHandlers() EList
}
