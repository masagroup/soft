package ecore

type xmiResourceLoad struct {
	xmlResourceLoad
}

type xmiResourceSave struct {
	xmlResourceSave
}

type XMIResource struct {
	*XMLResource
}

func NewXMIResource() *XMIResource {
	r := new(XMIResource)
	r.XMLResource = NewXMLResource()
	return r
}
