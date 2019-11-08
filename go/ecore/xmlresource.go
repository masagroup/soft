package ecore

import (
	"encoding/xml"
	"io"
	"net/url"
	"strings"

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

const (
	xsiURI                    = "http://www.w3.org/2001/XMLSchema-instance"
	xsiNS                     = "xsi"
	xmlNS                     = "xmlns"
	schemaLocation            = "schemaLocation"
	noNamespaceSchemaLocation = "noNamespaceSchemaLocation"
	typeAttrib                = "type"
	href                      = "href"
)

var notFeatures = [...]string{typeAttrib, schemaLocation, noNamespaceSchemaLocation}

const (
	single   = iota
	many     = iota
	manyAdd  = iota
	manyMove = iota
	other    = iota
)

type reference struct {
	object  EObject
	feature EStructuralFeature
	id      string
	pos     int
}

type xmlResourceLoader struct {
	decoder             *xml.Decoder
	resource            EResource
	isNamespaceAware    bool
	isResolveDeferred   bool
	elements            []string
	objects             []EObject
	attributes          []xml.Attr
	references          []reference
	namespaces          *xmlNamespaces
	prefixesToFactories map[string]EFactory
	sameDocumentProxies []EObject
}

func (l *xmlResourceLoader) startElement(e xml.StartElement) {
	l.setAttributes(e.Attr)
	l.namespaces.pushContext()
	l.handlePrefixMapping()
	if len(l.objects) == 0 {
		l.handleSchemaLocation()
	}
	l.processElement(e.Name.Space, e.Name.Local)
}

func (l *xmlResourceLoader) endElement(e xml.EndElement) {

	l.objects = l.objects[:len(l.objects)-1]
	if len(l.objects) == 0 {
		l.handleReferences()
	}

	prefixes := l.namespaces.popContext()
	for _, p := range prefixes {
		delete(l.prefixesToFactories, p[0].(string))
	}

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
	xsiSchemaLocation := l.getAttributeValue(xsiURI, schemaLocation)
	if len(xsiSchemaLocation) > 0 {
		l.handleXSISchemaLocation(xsiSchemaLocation)
	}

	xsiNoNamespaceSchemaLocation := l.getAttributeValue(xsiURI, noNamespaceSchemaLocation)
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
			if attr.Name.Space == xmlNS {
				l.startPrefixMapping(attr.Name.Local, attr.Value)
			}
		}
	}
}

func (l *xmlResourceLoader) startPrefixMapping(prefix string, uri string) {
	l.isNamespaceAware = true
	l.namespaces.declarePrefix(prefix, uri)
	delete(l.prefixesToFactories, prefix)
}

func (l *xmlResourceLoader) processElement(prefix string, local string) {
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

func (l *xmlResourceLoader) createObjectFromFeatureType(eObject EObject, eFeature EStructuralFeature) EObject {
	var eResult EObject
	if eFeature != nil && eFeature.GetEType() != nil {
		eType := eFeature.GetEType()
		eFactory := eType.GetEPackage().GetEFactoryInstance()
		eResult = l.createObjectWithFactory(eFactory, eType)
	}
	if eResult != nil {
		l.setFeatureValue(eObject, eFeature, eResult, -1)
		l.objects = append(l.objects, eResult)
	}
	return eResult
}

func (l *xmlResourceLoader) createObjectFromTypeName(eObject EObject, qname string, eFeature EStructuralFeature) EObject {
	prefix := ""
	local := qname
	if index := strings.Index(qname, ":"); index > 0 {
		prefix = qname[:index]
		local = qname[index+1:]
	}

	eFactory := l.getFactoryForPrefix(prefix)
	if eFactory == nil {
		l.handleUnknownPackage(prefix)
	}

	ePackage := eFactory.GetEPackage()
	eType := ePackage.GetEClassifier(local)
	eResult := l.createObjectWithFactory(eFactory, eType)
	if eResult != nil {
		l.setFeatureValue(eObject, eFeature, eResult, -1)
		l.objects = append(l.objects, eResult)
	}
	return eResult
}

func (l *xmlResourceLoader) handleFeature(prefix string, local string) {
	var eObject EObject
	if len(l.objects) > 0 {
		eObject = l.objects[len(l.objects)-1]
	}

	if eObject != nil {
		eFeature := l.getFeature(eObject, local)
		if eFeature != nil {
			xsiType := ""
			if l.isNamespaceAware {
				xsiType = l.getAttributeValue(xsiURI, typeAttrib)
			}
			if len(xsiType) > 0 {
				l.createObjectFromTypeName(eObject, xsiType, eFeature)
			} else {
				l.createObjectFromFeatureType(eObject, eFeature)
			}
		} else {
			l.handleUnknownFeature(local)
		}
	} else {
		l.handleUnknownFeature(local)
	}

}

func (l *xmlResourceLoader) setFeatureValue(eObject EObject,
	eFeature EStructuralFeature,
	value interface{},
	position int) {
	kind := l.getFeatureKind(eFeature)
	switch kind {
	case single:
		eClassifier := eFeature.GetEType()
		eDataType := eClassifier.(EDataType)
		eFactory := eDataType.GetEPackage().GetEFactoryInstance()
		if value == nil {
			eObject.ESet(eFeature, nil)
		} else {
			eObject.ESet(eFeature, eFactory.CreateFromString(eDataType, value.(string)))
		}

	case many:
		eClassifier := eFeature.GetEType()
		eDataType := eClassifier.(EDataType)
		eFactory := eDataType.GetEPackage().GetEFactoryInstance()
		eList := eObject.EGet(eFeature).(EList)
		if position == -2 {
		} else if value == nil {
			eList.Add(nil)
		} else {
			eList.Add(eFactory.CreateFromString(eDataType, value.(string)))
		}
	case manyAdd:
	case manyMove:
		eList := eObject.EGet(eFeature).(EList)
		if position == -1 {
			eList.Add(value)
		} else if position == -2 {
			eList.Clear()
		} else if eObject == value {
			index := eList.IndexOf(value)
			if index == -1 {
				eList.Insert(position, value)
			} else {
				eList.Move(position, index)
			}
		} else if kind == manyAdd {
			eList.Add(value)
		} else {
			eList.MoveObject(position, value)
		}
	default:
		eObject.ESet(eFeature, value)
	}
}

func (l *xmlResourceLoader) getFeatureKind(eFeature EStructuralFeature) int {
	eClassifier := eFeature.GetEType()
	if eDataType, _ := eClassifier.(EDataType); eDataType != nil {
		if eFeature.IsMany() {
			return many
		}
		return single
	} else if eFeature.IsMany() {
		eReference := eFeature.(EReference)
		eOpposite := eReference.GetEOpposite()
		if eOpposite == nil || eOpposite.IsTransient() || !eOpposite.IsMany() {
			return manyAdd
		}
		return manyMove
	}
	return other
}

func (l *xmlResourceLoader) handleAttributes(eObject EObject) {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			name := attr.Name.Local
			uri := attr.Name.Space
			value := attr.Value
			if name == href {
				l.handleProxy(eObject, value)
			} else if l.isNamespaceAware {
				if uri != xsiURI {
					l.setAttributeValue(eObject, name, value)
				}
			} else if uri != xmlNS {
				found := true
				for _, notFeature := range notFeatures {
					if notFeature == name {
						found = false
						break
					}
				}
				if !found {
					l.setAttributeValue(eObject, name, value)
				}
			}
		}
	}
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

func (l *xmlResourceLoader) setAttributeValue(eObject EObject, qname string, value string) {
	local := qname
	if index := strings.Index(qname, ":"); index > 0 {
		local = qname[index+1:]
	}
	eFeature := l.getFeature(eObject, local)
	if eFeature != nil {
		kind := l.getFeatureKind(eFeature)
		if kind == single || kind == many {
			l.setFeatureValue(eObject, eFeature, value, -2)
		} else {
			l.setValueFromId(eObject, eFeature.(EReference), value)
		}
	} else {
		l.handleUnknownFeature(local)
	}
}

func (l *xmlResourceLoader) setValueFromId(eObject EObject, eReference EReference, ids string) {
	mustAdd := l.isResolveDeferred
	mustAddOrNotOppositeIsMany := false
	isFirstID := true
	position := 0
	references := []reference{}
	tokens := strings.Split(ids, " ")
	qName := ""
	for _, token := range tokens {
		id := ""

		if index := strings.Index(token, "#"); index != -1 {
			if index == 0 {
				id = token[1:]
			} else {
				oldAttributes := l.setAttributes(nil)
				var eProxy EObject
				if len(qName) == 0 {
					eProxy = l.createObjectFromFeatureType(eObject, eReference)
				} else {
					eProxy = l.createObjectFromTypeName(eObject, qName, eReference)
				}
				l.setAttributes(oldAttributes)
				if eProxy != nil {
					l.handleProxy(eProxy, id)
				}
				l.objects = l.objects[:len(l.objects)-1]
				qName = ""
				position++
				continue
			}
		} else if index := strings.Index(token, ":"); index != -1 {
			qName = token
			continue
		}

		if !l.isResolveDeferred {
			if isFirstID {
				eOpposite := eReference.GetEOpposite()
				if eOpposite != nil {
					mustAdd = eOpposite.IsTransient() || eReference.IsMany()
					mustAddOrNotOppositeIsMany = mustAdd || !eOpposite.IsMany()
				} else {
					mustAdd = true
					mustAddOrNotOppositeIsMany = true
				}
				isFirstID = false
			}

			if mustAddOrNotOppositeIsMany {
				resolved := l.resource.GetEObject(id)
				if resolved != nil {
					l.setFeatureValue(eObject, eReference, resolved, -1)
					qName = ""
					position++
					continue
				}
			}
		}

		if mustAdd {
			references = append(references, reference{object: eObject, feature: eReference, id: id, pos: position})
		}

		qName = ""
		position++
	}

	if position == 0 {
		l.setFeatureValue(eObject, eReference, nil, -2)
	} else {
		l.references = references
	}
}

func (l *xmlResourceLoader) handleProxy(eProxy EObject, id string) {
	uri, ok := url.Parse(id)
	if ok != nil {
		return
	}

	eProxy.(EObjectInternal).ESetProxyURI(uri)

	if (l.resource.GetURI() == &url.URL{Scheme: uri.Scheme,
		User:       uri.User,
		Host:       uri.Host,
		Path:       uri.Path,
		ForceQuery: uri.ForceQuery,
		RawPath:    uri.RawPath,
		RawQuery:   uri.RawQuery}) {
		l.sameDocumentProxies = append(l.sameDocumentProxies, eProxy)
	}
}

func (l *xmlResourceLoader) handleReferences() {

}

func (l *xmlResourceLoader) getFeature(eObject EObject, name string) EStructuralFeature {
	eClass := eObject.EClass()
	eFeature := eClass.GetEStructuralFeatureFromString(name)
	return eFeature
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
