package ecore

type xmiResourceImpl struct {
	*xmlResourceImpl
}

func newXMIResourceImpl() *xmiResourceImpl {
	r := new(xmiResourceImpl)
	r.xmlResourceImpl = newXMLResourceImpl()
	return r
}
