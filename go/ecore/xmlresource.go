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
	resource      EResource
	isRoot        bool
	isPushContext bool
	elements      []string
	objects       []interface{}
	namespaces    *xmlNamespaces
}

func (l *xmlResourceLoader) startElement(e xml.StartElement) {

}

func (l *xmlResourceLoader) endElement(e xml.EndElement) {

}

func (l *xmlResourceLoader) charData(data string) {
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
		resource:      r,
		isRoot:        true,
		isPushContext: true,
		namespaces:    newXmlNamespaces(),
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
			loader.charData(string([]byte(t)))
		case xml.Comment:
			loader.comment(string([]byte(t)))
		case xml.ProcInst:
			loader.processingInstruction(t)
		case xml.Directive:
			loader.directive(string([]byte(t)))
		}
	}
}
