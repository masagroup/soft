package ecore

import (
	"io"
	"net/url"
	"os"
)

//URIHandler ...
type FileURIHandler struct {
}

func (fuh *FileURIHandler) CanHandle(uri *url.URL) bool {
	return uri.Scheme == "file" || (len(uri.Scheme) == 0 && len(uri.Host) == 0 && len(uri.RawQuery) == 0)
}

func (fuh *FileURIHandler) CreateReader(uri *url.URL) io.Reader {
	f, _ := os.Create(uri.String())
	return f
}

func (fuh *FileURIHandler) CreateWriter(uri *url.URL) io.Writer {
	f, _ := os.Create(uri.String())
	return f
}
