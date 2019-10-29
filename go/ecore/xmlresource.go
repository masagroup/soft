package ecore

import "io"

type XMLResource struct {
	*EResourceImpl
}

func NewXMLResource() *XMLResource {
	r := new(XMLResource)
	r.EResourceImpl = NewEResourceImpl()
	r.SetInterfaces(r)
	return r
}

func (r *XMLResource) DoLoad(rd io.Reader) {

}
