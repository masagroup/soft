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
	loader := &xmlResourceLoader{
		resource:            r,
		isRoot:              true,
		isPushContext:       true,
		namespaces:          newXmlNamespaces(),
		prefixesToFactories: make(map[string]EFactory),
	}

	d := xml.NewDecoder(rd)
	d.CharsetReader = charset.NewReaderLabel
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
