package ecore

type xmiLoadImpl struct {
	*xmlLoadImpl
}

func newXMILoadImpl() *xmiLoadImpl {
	return &xmiLoadImpl{xmlLoadImpl: newXMLLoadImpl()}
}

type xmiSaveImpl struct {
	*xmlSaveImpl
}

func newXMISaveImpl() *xmiSaveImpl {
	return &xmiSaveImpl{xmlSaveImpl: newXMLSaveImpl()}
}

type xmiResourceImpl struct {
	*xmlResourceImpl
}

func newXMIResourceImpl() *xmiResourceImpl {
	r := new(xmiResourceImpl)
	r.xmlResourceImpl = newXMLResourceImpl()
	r.SetInterfaces(r)
	return r
}

func (r *xmiResourceImpl) createLoad() xmlLoad {
	return newXMILoadImpl()
}

func (r *xmiResourceImpl) createSave() xmlSave {
	return newXMISaveImpl()
}
