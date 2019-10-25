package ecore

import (
	"io"
	"net/url"
)

const (
	RESOURCE__RESOURCE_SET = 0

	RESOURCE__URI = 1

	RESOURCE__CONTENTS = 2

	RESOURCE__IS_LOADED = 4
)

//EResource ...
type EResource interface {
	ENotifier

	GetResourceSet() EResourceSet

	GetURI() *url.URL
	SetURI(*url.URL)

	GetContents() EList
	GetAllContents() EIterator

	GetEObject(string) EObject
	GetURIFragment(EObject) string

	Attached(object EObject)
	Detached(object EObject)

	Load()
	LoadReader(r io.Reader)
	Unload()
	IsLoaded() bool

	Save()
	SaveWriter(w io.Writer)

	GetErrors() EList
	GetWarnings() EList
}
