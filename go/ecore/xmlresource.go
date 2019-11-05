package ecore

import (
	"encoding/xml"
	"io"

	"golang.org/x/net/html/charset"
)

type pair [2]interface{}

type xmlNamespaces struct {
	namespaces     []pair
	namespacesSize int
	currentContext int
	contexts       []int
}

func newXmlNamespaces() *xmlNamespaces {
	n := &xmlNamespaces{
		currentContext: -1,
	}
	return n
}

func (n *xmlNamespaces) pushContext() {
	n.currentContext++
	if n.currentContext >= len(n.contexts) {
		n.contexts = append(n.contexts, n.namespacesSize)
	} else {
		n.contexts[n.currentContext] = n.namespacesSize
	}
}

func (n *xmlNamespaces) popContext() []pair {
	oldPrefixSize := n.namespacesSize
	n.namespacesSize = n.contexts[n.currentContext]
	n.currentContext--
	return n.namespaces[n.namespacesSize:oldPrefixSize]
}

func (n *xmlNamespaces) declarePrefix(prefix string, uri string) bool {
	for i := n.namespacesSize; i > n.contexts[n.currentContext]; i-- {
		p := &n.namespaces[i-1]
		if p[0] == prefix {
			p[1] = uri
			return true
		}
	}
	n.namespacesSize++
	if n.namespacesSize > len(n.namespaces) {
		n.namespaces = append(n.namespaces, pair{prefix, uri})
	} else {
		n.namespaces[n.namespacesSize] = pair{prefix, uri}
	}
	return false
}

func (n *xmlNamespaces) getPrefix(uri string) string {
	for i := n.namespacesSize; i > 0; i-- {
		p := n.namespaces[i-1]
		if p[1].(string) == uri {
			return p[0].(string)
		}
	}
	return ""
}

func (n *xmlNamespaces) getURI(prefix string) string {
	for i := n.namespacesSize; i > 0; i-- {
		p := n.namespaces[i-1]
		if p[0].(string) == prefix {
			return p[1].(string)
		}
	}
	return ""
}

type xmlResourceLoader struct {
	decoder             *xml.Decoder
	resource            EResource
	isRoot              bool
	isPushContext       bool
	isNamespaceAware    bool
	elements            []string
	objects             []interface{}
	attributes          []xml.Attr
	namespaces          *xmlNamespaces
	prefixesToFactories map[string]EFactory
}

func (l *xmlResourceLoader) startElement(e xml.StartElement) {
	l.setAttributes(e.Attr)

	if l.isRoot {
		l.handleSchemaLocation()
		l.handlePrefixMapping()
	}

	if l.isPushContext {
		l.namespaces.pushContext()
	}
	l.isPushContext = true

}

func (l *xmlResourceLoader) setAttributes(attributes []xml.Attr) []xml.Attr {
	old := l.attributes
	l.attributes = attributes
	return old
}

func (l *xmlResourceLoader) getAttributeValue(uri string, local string) string {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			if attr.Name.Space == uri && attr.Name.Local == local {
				return attr.Value
			}
		}
	}
	return ""
}

func (l *xmlResourceLoader) handleSchemaLocation() {
	xsiSchemaLocation := l.getAttributeValue("http://www.w3.org/2001/XMLSchema-instance", "schemaLocation")
	if len(xsiSchemaLocation) > 0 {
		l.handleXSISchemaLocation(xsiSchemaLocation)
	}

	xsiNoNamespaceSchemaLocation := l.getAttributeValue("http://www.w3.org/2001/XMLSchema-instance", "noNamespaceSchemaLocation")
	if len(xsiNoNamespaceSchemaLocation) > 0 {
		l.handleXSINoNamespaceSchemaLocation(xsiNoNamespaceSchemaLocation)
	}
}

func (l *xmlResourceLoader) handleXSISchemaLocation(loc string) {
}

func (l *xmlResourceLoader) handleXSINoNamespaceSchemaLocation(loc string) {
}

func (l *xmlResourceLoader) handlePrefixMapping() {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			if attr.Name.Space == "xmlns" {
				l.startPrefixMapping(attr.Name.Local, attr.Value)
			}
		}
	}
}

func (l *xmlResourceLoader) startPrefixMapping(prefix string, uri string) {
	l.isNamespaceAware = true
	if l.isPushContext {
		l.namespaces.pushContext()
		l.isPushContext = false
	}
	l.namespaces.declarePrefix(prefix, uri)
	delete(l.prefixesToFactories, prefix)
}

func (l *xmlResourceLoader) processElement(prefix string, local string) {
	l.isRoot = false
	if len(l.objects) == 0 {
		eObject := l.createObject(prefix, local)
		if eObject != nil {
			l.objects = append(l.objects, eObject)
			l.resource.GetContents().Add(eObject)
		}
	} else {
		l.handleFeature(prefix, local)
	}
}

func (l *xmlResourceLoader) handleFeature(prefix string, local string) {

}

func (l *xmlResourceLoader) createObject(prefix string, local string) EObject {
	eFactory := l.getFactoryForPrefix(prefix)
	if eFactory != nil {
		ePackage := eFactory.GetEPackage()
		eType := ePackage.GetEClassifier(local)
		return l.createObjectWithFactory(eFactory, eType)
	} else {
		l.handleUnknownPackage(l.namespaces.getURI(prefix))
		return nil
	}
}

func (l *xmlResourceLoader) createObjectWithFactory(eFactory EFactory, eType EClassifier) EObject {
	if eFactory != nil {
		eClass := eType.(EClass)
		if eClass != nil && !eClass.IsAbstract() {
			eObject := eFactory.Create(eClass)
			if eObject != nil {
				l.handleAttributes(eObject)
			}
			return eObject
		}
	}
	return nil
}

func (l *xmlResourceLoader) handleAttributes(eObject EObject) {
}

func (l *xmlResourceLoader) getFactoryForPrefix(prefix string) EFactory {

	factory, _ := l.prefixesToFactories[prefix]
	if factory == nil {
		packageRegistry := GetPackageRegistry()
		if l.resource.GetResourceSet() != nil {
			packageRegistry = l.resource.GetResourceSet().GetPackageRegistry()
		}
		uri := l.namespaces.getURI(prefix)
		factory = packageRegistry.GetFactory(uri)
		if factory != nil {
			l.prefixesToFactories[prefix] = factory
		}
	}
	return factory
}

func (l *xmlResourceLoader) handleUnknownFeature(name string) {
	l.error(NewEDiagnosticImpl("Feature "+name+"not found", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
}

func (l *xmlResourceLoader) handleUnknownPackage(name string) {
	l.error(NewEDiagnosticImpl("Package "+name+"not found", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
}

func (l *xmlResourceLoader) error(diagnostic EDiagnostic) {
	l.resource.GetErrors().Add(diagnostic)
}

func (l *xmlResourceLoader) warning(diagnostic EDiagnostic) {
	l.resource.GetWarnings().Add(diagnostic)
}

func (l *xmlResourceLoader) endElement(e xml.EndElement) {

}

func (l *xmlResourceLoader) text(data string) {
}

func (l *xmlResourceLoader) comment(comment string) {
}

func (l *xmlResourceLoader) processingInstruction(procInst xml.ProcInst) {
}

func (l *xmlResourceLoader) directive(directive string) {
}

func NewXMLResource() *XMLResource {
	r := new(XMLResource)
	r.EResourceImpl = NewEResourceImpl()
	r.SetInterfaces(r)
	return r
}

type XMLResource struct {
	*EResourceImpl
}

func (r *XMLResource) DoLoad(rd io.Reader) {

	d := xml.NewDecoder(rd)
	d.CharsetReader = charset.NewReaderLabel

	loader := &xmlResourceLoader{
		decoder:             d,
		resource:            r,
		isRoot:              true,
		isPushContext:       true,
		namespaces:          newXmlNamespaces(),
		prefixesToFactories: make(map[string]EFactory),
	}

	for {
		t, tokenErr := d.Token()
		if tokenErr != nil {
			if tokenErr == io.EOF {
				break
			}
			// handle error
		}
		switch t := t.(type) {
		case xml.StartElement:
			loader.startElement(t)
		case xml.EndElement:
			loader.endElement(t)
		case xml.CharData:
			loader.text(string([]byte(t)))
		case xml.Comment:
			loader.comment(string([]byte(t)))
		case xml.ProcInst:
			loader.processingInstruction(t)
		case xml.Directive:
			loader.directive(string([]byte(t)))
		}
	}
}
