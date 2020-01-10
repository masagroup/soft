package ecore

import "encoding/xml"

const (
	xmiURI        = "http://www.omg.org/XMI"
	xmiNS         = "xmi"
	versionAttrib = "version"
	uuidAttrib    = "uuid"
)

type xmiLoadImpl struct {
	*xmlLoadImpl
}

func newXMILoadImpl() *xmiLoadImpl {
	l := new(xmiLoadImpl)
	l.xmlLoadImpl = newXMLLoadImpl()
	l.notFeatures = append(l.notFeatures, xml.Name{Space: xmiURI, Local: typeAttrib}, xml.Name{Space: xmiURI, Local: versionAttrib}, xml.Name{Space: xmiURI, Local: uuidAttrib})
	l.interfaces = l
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
	version := l.getAttributeValue(xmiURI, versionAttrib)
	if len(version) > 0 {
		l.resource.(XMIResource).SetXMIVersion(version)
	}
	l.xmlLoadImpl.handleAttributes(object)
}

type xmiSaveImpl struct {
	*xmlSaveImpl
}

func newXMISaveImpl() *xmiSaveImpl {
	s := new(xmiSaveImpl)
	s.xmlSaveImpl = newXMLSaveImpl()
	s.interfaces = s
	return s
}

func (s *xmiSaveImpl) saveNamespaces() {
	s.str.addAttribute(xmiNS+":"+versionAttrib, s.resource.(XMIResource).GetXMIVersion())
	s.str.addAttribute(xmlNS+":"+xmiNS, xmiURI)
	s.xmlSaveImpl.saveNamespaces()
}

type XMIResource interface {
	XMLResource

	SetXMIVersion(version string)
	GetXMIVersion() string
}

type xmiResourceImpl struct {
	*xmlResourceImpl
	xmiVersion string
}

func newXMIResourceImpl() *xmiResourceImpl {
	r := new(xmiResourceImpl)
	r.xmlResourceImpl = newXMLResourceImpl()
	r.SetInterfaces(r)
	return r
}

func (r *xmiResourceImpl) SetXMIVersion(xmiVersion string) {
	r.xmiVersion = xmiVersion
}

func (r *xmiResourceImpl) GetXMIVersion() string {
	return r.xmiVersion
}

func (r *xmiResourceImpl) createLoad() xmlLoad {
	return newXMILoadImpl()
}

func (r *xmiResourceImpl) createSave() xmlSave {
	return newXMISaveImpl()
}
