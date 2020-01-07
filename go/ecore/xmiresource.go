package ecore

const (
	xmiURI = "http://www.w3.org/2001/XMLSchema-instance"
	xmiNS  = "xmi"
)

type xmiLoadImpl struct {
	*xmlLoadImpl
}

func newXMILoadImpl() *xmiLoadImpl {
	l := new(xmiLoadImpl)
	l.interfaces = l
	l.xmlLoadImpl = newXMLLoadImpl()
	return l
}

func (l *xmiLoadImpl) getXSIType() string {
	xsiType := l.xmlLoadImpl.getXSIType()
	if len(xsiType) == 0 && l.attributes != nil {
		return l.getAttributeValue(xmiURI, typeAttrib)
	}
	return xsiType
}

func (l *xmiLoadImpl) handleAttributes(object EObject) {

}

type xmiSaveImpl struct {
	*xmlSaveImpl
}

func newXMISaveImpl() *xmiSaveImpl {
	return &xmiSaveImpl{xmlSaveImpl: newXMLSaveImpl()}
}

type XMIResource interface {
	XMLResource
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
