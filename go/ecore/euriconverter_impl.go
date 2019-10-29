package ecore

import (
	"io"
	"net/url"
)

type EURIConverterImpl struct {
	uriHandlers EList
}

func NewEURIConverterImpl() *EURIConverterImpl {
	r := new(EURIConverterImpl)
	r.uriHandlers = NewImmutableEList([]interface{}{new(FileURIHandler)})
	return r
}

func (r *EURIConverterImpl) CreateReader(uri *url.URL) io.ReadCloser {
	uriHandler := r.GetURIHandler(uri)
	if uriHandler != nil {
		return uriHandler.CreateReader(uri)
	}
	return nil
}

func (r *EURIConverterImpl) CreateWriter(uri *url.URL) io.WriteCloser {
	uriHandler := r.GetURIHandler(uri)
	if uriHandler != nil {
		return uriHandler.CreateWriter(uri)
	}
	return nil
}

func (r *EURIConverterImpl) Normalize(uri *url.URL) *url.URL {
	return uri
}

func (r *EURIConverterImpl) GetURIHandler(uri *url.URL) EURIHandler {
	for it := r.uriHandlers.Iterator(); it.HasNext(); {
		uriHandler := it.Next().(EURIHandler)
		if uriHandler.CanHandle(uri) {
			return uriHandler
		}
	}
	return nil
}

func (r *EURIConverterImpl) GetURIHandlers() EList {
	return r.uriHandlers
}
