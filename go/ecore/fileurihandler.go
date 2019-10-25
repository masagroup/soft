package ecore

import (
	"io"
	"net/url"
	"os"
)

//URIHandler ...
type FileURIHandler struct {
}

func (fuh *FileURIHandler) createInputStream(uri *url.URL) io.Reader {
	f, _ := os.Create(uri.String())
	return f
}

func (fuh *FileURIHandler) createOutputStream(uri *url.URL) io.Writer {
	f, _ := os.Create(uri.String())
	return f
}
