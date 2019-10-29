package ecore

import (
	"io"
	"net/url"
	"os"
)

//URIHandler ...
type FileURIHandler struct {
}

func (fuh *FileURIHandler) CreateReader(uri *url.URL) io.Reader {
	f, _ := os.Create(uri.String())
	return f
}

func (fuh *FileURIHandler) CreateWriter(uri *url.URL) io.Writer {
	f, _ := os.Create(uri.String())
	return f
}
