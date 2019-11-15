package ecore

import (
	"io"
	"net/url"
)

//URIConverter ...
type EURIConverter interface {
	CreateReader(uri *url.URL) io.ReadCloser

	CreateWriter(uri *url.URL) io.WriteCloser

	Normalize(uri *url.URL) *url.URL

	GetURIHandler(uri *url.URL) EURIHandler

	GetURIHandlers() EList
}
