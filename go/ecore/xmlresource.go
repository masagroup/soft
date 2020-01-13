package ecore

import (
	"encoding/xml"
	"fmt"
	"io"
	"net/url"
	"sort"
	"strings"

	"golang.org/x/net/html/charset"
)

type xmlLoad interface {
	load(resource XMLResource, w io.Reader)
}

type xmlLoadInternal interface {
	getXSIType() string
	handleAttributes(object EObject)
}

type xmlSave interface {
	save(resource XMLResource, w io.Writer)
}

type xmlSaveInternal interface {
	saveNamespaces()
}

type XMLResource interface {
	EResourceInternal
	createLoad() xmlLoad
	createSave() xmlSave
}

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

func (n *xmlNamespaces) getPrefix(uri string) (response string, ok bool) {
	for i := n.namespacesSize; i > 0; i-- {
		p := n.namespaces[i-1]
		if p[1].(string) == uri {
			return p[0].(string), true
		}
	}
	return "", false
}

func (n *xmlNamespaces) getURI(prefix string) (response string, ok bool) {
	for i := n.namespacesSize; i > 0; i-- {
		p := n.namespaces[i-1]
		if p[0].(string) == prefix {
			return p[1].(string), true
		}
	}
	return "", false
}

const (
	href                            = "href"
	typeAttrib                      = "type"
	schemaLocationAttrib            = "schemaLocation"
	noNamespaceSchemaLocationAttrib = "noNamespaceSchemaLocation"
	xsiURI                          = "http://www.w3.org/2001/XMLSchema-instance"
	xsiNS                           = "xsi"
	xmlNS                           = "xmlns"
)

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

type xmlLoadImpl struct {
	interfaces          interface{}
	decoder             *xml.Decoder
	resource            XMLResource
	isResolveDeferred   bool
	elements            []string
	objects             []EObject
	attributes          []xml.Attr
	references          []reference
	namespaces          *xmlNamespaces
	spacesToFactories   map[string]EFactory
	sameDocumentProxies []EObject
	notFeatures         []xml.Name
}

func newXMLLoadImpl() *xmlLoadImpl {
	l := new(xmlLoadImpl)
	l.interfaces = l
	l.namespaces = newXmlNamespaces()
	l.spacesToFactories = make(map[string]EFactory)
	l.notFeatures = append(l.notFeatures, xml.Name{Space: xsiURI, Local: typeAttrib}, xml.Name{Space: xsiURI, Local: schemaLocationAttrib}, xml.Name{Space: xsiURI, Local: noNamespaceSchemaLocationAttrib})
	return l
}

func (l *xmlLoadImpl) load(resource XMLResource, r io.Reader) {
	l.decoder = xml.NewDecoder(r)
	l.decoder.CharsetReader = charset.NewReaderLabel
	l.resource = resource

	for {
		t, tokenErr := l.decoder.Token()
		if tokenErr != nil {
			if tokenErr == io.EOF {
				break
			}
			// handle error
		}
		switch t := t.(type) {
		case xml.StartElement:
			l.startElement(t)
		case xml.EndElement:
			l.endElement(t)
		case xml.CharData:
			l.text(string([]byte(t)))
		case xml.Comment:
			l.comment(string([]byte(t)))
		case xml.ProcInst:
			l.processingInstruction(t)
		case xml.Directive:
			l.directive(string([]byte(t)))
		}
	}
}

func (l *xmlLoadImpl) startElement(e xml.StartElement) {
	l.setAttributes(e.Attr)
	l.namespaces.pushContext()
	l.handlePrefixMapping()
	if len(l.objects) == 0 {
		l.handleSchemaLocation()
	}
	l.processElement(e.Name.Space, e.Name.Local)
}

func (l *xmlLoadImpl) endElement(e xml.EndElement) {

	if len(l.objects) > 0 {
		l.objects = l.objects[:len(l.objects)-1]
	}
	if len(l.objects) == 0 {
		l.handleReferences()
	}

	context := l.namespaces.popContext()
	for _, p := range context {
		delete(l.spacesToFactories, p[1].(string))
	}

}

func (l *xmlLoadImpl) setAttributes(attributes []xml.Attr) []xml.Attr {
	old := l.attributes
	l.attributes = attributes
	return old
}

func (l *xmlLoadImpl) getAttributeValue(uri string, local string) string {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			if attr.Name.Space == uri && attr.Name.Local == local {
				return attr.Value
			}
		}
	}
	return ""
}

func (l *xmlLoadImpl) getXSIType() string {
	return l.getAttributeValue(xsiURI, typeAttrib)
}

func (l *xmlLoadImpl) handleSchemaLocation() {
	xsiSchemaLocation := l.getAttributeValue(xsiURI, schemaLocationAttrib)
	if len(xsiSchemaLocation) > 0 {
		l.handleXSISchemaLocation(xsiSchemaLocation)
	}

	xsiNoNamespaceSchemaLocation := l.getAttributeValue(xsiURI, noNamespaceSchemaLocationAttrib)
	if len(xsiNoNamespaceSchemaLocation) > 0 {
		l.handleXSINoNamespaceSchemaLocation(xsiNoNamespaceSchemaLocation)
	}
}

func (l *xmlLoadImpl) handleXSISchemaLocation(loc string) {
}

func (l *xmlLoadImpl) handleXSINoNamespaceSchemaLocation(loc string) {
}

func (l *xmlLoadImpl) handlePrefixMapping() {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			if attr.Name.Space == xmlNS {
				l.startPrefixMapping(attr.Name.Local, attr.Value)
			}
		}
	}
}

func (l *xmlLoadImpl) startPrefixMapping(prefix string, uri string) {
	l.namespaces.declarePrefix(prefix, uri)
	delete(l.spacesToFactories, uri)
}

func (l *xmlLoadImpl) processElement(space string, local string) {
	if len(l.objects) == 0 {
		eObject := l.createObject(space, local)
		if eObject != nil {
			l.objects = append(l.objects, eObject)
			l.resource.GetContents().Add(eObject)
		}
	} else {
		l.handleFeature(space, local)
	}
}

func (l *xmlLoadImpl) createObject(space string, local string) EObject {
	eFactory := l.getFactoryForSpace(space)
	if eFactory != nil {
		ePackage := eFactory.GetEPackage()
		eType := ePackage.GetEClassifier(local)
		return l.createObjectWithFactory(eFactory, eType)
	} else {
		prefix, _ := l.namespaces.getPrefix(space)
		l.handleUnknownPackage(prefix)
		return nil
	}
}

func (l *xmlLoadImpl) createObjectWithFactory(eFactory EFactory, eType EClassifier) EObject {
	if eFactory != nil {
		eClass, _ := eType.(EClass)
		if eClass != nil && !eClass.IsAbstract() {
			eObject := eFactory.Create(eClass)
			if eObject != nil {
				l.interfaces.(xmlLoadInternal).handleAttributes(eObject)
			}
			return eObject
		}
	}
	return nil
}

func (l *xmlLoadImpl) createObjectFromFeatureType(eObject EObject, eFeature EStructuralFeature) EObject {
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

func (l *xmlLoadImpl) createObjectFromTypeName(eObject EObject, qname string, eFeature EStructuralFeature) EObject {
	prefix := ""
	local := qname
	if index := strings.Index(qname, ":"); index > 0 {
		prefix = qname[:index]
		local = qname[index+1:]
	}
	space, _ := l.namespaces.getURI(prefix)
	eFactory := l.getFactoryForSpace(space)
	if eFactory == nil {
		l.handleUnknownPackage(prefix)
		return nil
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

func (l *xmlLoadImpl) handleFeature(space string, local string) {
	var eObject EObject
	if len(l.objects) > 0 {
		eObject = l.objects[len(l.objects)-1]
	}

	if eObject != nil {
		eFeature := l.getFeature(eObject, local)
		if eFeature != nil {
			xsiType := l.interfaces.(xmlLoadInternal).getXSIType()
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

func (l *xmlLoadImpl) setFeatureValue(eObject EObject,
	eFeature EStructuralFeature,
	value interface{},
	position int) {
	kind := l.getLoadFeatureKind(eFeature)
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
		fallthrough
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

func (l *xmlLoadImpl) getLoadFeatureKind(eFeature EStructuralFeature) int {
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

func (l *xmlLoadImpl) handleAttributes(eObject EObject) {
	if l.attributes != nil {
		for _, attr := range l.attributes {
			name := attr.Name.Local
			uri := attr.Name.Space
			value := attr.Value
			if name == href {
				l.handleProxy(eObject, value)
			} else if uri != xmlNS && l.isUserAttribute(attr.Name) {
				l.setAttributeValue(eObject, name, value)
			}
		}
	}
}

func (l *xmlLoadImpl) isUserAttribute(name xml.Name) bool {
	for _, notFeature := range l.notFeatures {
		if notFeature == name {
			return false
		}
	}
	return true
}

func (l *xmlLoadImpl) getFactoryForSpace(space string) EFactory {
	factory, _ := l.spacesToFactories[space]
	if factory == nil {
		packageRegistry := GetPackageRegistry()
		if l.resource.GetResourceSet() != nil {
			packageRegistry = l.resource.GetResourceSet().GetPackageRegistry()
		}
		factory = packageRegistry.GetFactory(space)
		if factory != nil {
			l.spacesToFactories[space] = factory
		}
	}
	return factory
}

func (l *xmlLoadImpl) setAttributeValue(eObject EObject, qname string, value string) {
	local := qname
	if index := strings.Index(qname, ":"); index > 0 {
		local = qname[index+1:]
	}
	eFeature := l.getFeature(eObject, local)
	if eFeature != nil {
		kind := l.getLoadFeatureKind(eFeature)
		if kind == single || kind == many {
			l.setFeatureValue(eObject, eFeature, value, -2)
		} else {
			l.setValueFromId(eObject, eFeature.(EReference), value)
		}
	} else {
		l.handleUnknownFeature(local)
	}
}

func (l *xmlLoadImpl) setValueFromId(eObject EObject, eReference EReference, ids string) {
	mustAdd := l.isResolveDeferred
	mustAddOrNotOppositeIsMany := false
	isFirstID := true
	position := 0
	references := []reference{}
	tokens := strings.Split(ids, " ")
	qName := ""
	for _, token := range tokens {
		id := token
		if index := strings.Index(id, "#"); index != -1 {
			if index == 0 {
				id = id[1:]
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
		} else if index := strings.Index(id, ":"); index != -1 {
			qName = id
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
		l.references = append(l.references, references...)
	}
}

func (l *xmlLoadImpl) handleProxy(eProxy EObject, id string) {
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

func (l *xmlLoadImpl) handleReferences() {
	for _, eProxy := range l.sameDocumentProxies {
		for itRef := eProxy.EClass().GetEAllReferences().Iterator(); itRef.HasNext(); {
			eReference := itRef.Next().(EReference)
			eOpposite := eReference.GetEOpposite()
			if eOpposite != nil && eOpposite.IsChangeable() && eProxy.EIsSet(eReference) {
				resolvedObject := l.resource.GetEObject(eProxy.(EObjectInternal).EProxyURI().Fragment)
				if resolvedObject != nil {
					var proxyHolder EObject
					if eReference.IsMany() {
						value := eProxy.EGet(eReference)
						list := value.(EList)
						proxyHolder = list.Get(0).(EObject)
					} else {
						value := eProxy.EGet(eReference)
						proxyHolder = value.(EObject)
					}

					if eOpposite.IsMany() {
						value := proxyHolder.EGet(eOpposite)
						holderContents := value.(EList)
						resolvedIndex := holderContents.IndexOf(resolvedObject)
						if resolvedIndex != -1 {
							proxyIndex := holderContents.IndexOf(eProxy)
							holderContents.Move(proxyIndex, resolvedIndex)
							if proxyIndex > resolvedIndex {
								holderContents.Remove(proxyIndex - 1)
							} else {
								holderContents.Remove(proxyIndex + 1)
							}
							break
						}
					}

					replace := false
					if eReference.IsMany() {
						value := resolvedObject.EGet(eReference)
						list := value.(EList)
						replace = !list.Contains(proxyHolder)
					} else {
						value := resolvedObject.EGet(eReference)
						object := value.(EObject)
						replace = object != proxyHolder
					}

					if replace {
						if eOpposite.IsMany() {
							value := proxyHolder.EGet(eOpposite)
							list := value.(EList)
							ndx := list.IndexOf(eProxy)
							list.Set(ndx, resolvedObject)
						} else {
							proxyHolder.ESet(eOpposite, resolvedObject)
						}
					}
					break
				}
			}
		}
	}

	for _, reference := range l.references {
		eObject := l.resource.GetEObject(reference.id)
		if eObject != nil {
			l.setFeatureValue(reference.object, reference.feature, eObject, reference.pos)
		} else {
			l.error(NewEDiagnosticImpl(
				"Unresolved reference '"+reference.id+"'", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
		}
	}
}

func (l *xmlLoadImpl) getFeature(eObject EObject, name string) EStructuralFeature {
	eClass := eObject.EClass()
	eFeature := eClass.GetEStructuralFeatureFromString(name)
	return eFeature
}

func (l *xmlLoadImpl) handleUnknownFeature(name string) {
	l.error(NewEDiagnosticImpl("Feature "+name+" not found", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
}

func (l *xmlLoadImpl) handleUnknownPackage(name string) {
	l.error(NewEDiagnosticImpl("Package "+name+" not found", l.resource.GetURI().String(), int(l.decoder.InputOffset()), 0))
}

func (l *xmlLoadImpl) error(diagnostic EDiagnostic) {
	l.resource.GetErrors().Add(diagnostic)
}

func (l *xmlLoadImpl) warning(diagnostic EDiagnostic) {
	l.resource.GetWarnings().Add(diagnostic)
}

func (l *xmlLoadImpl) text(data string) {
}

func (l *xmlLoadImpl) comment(comment string) {
}

func (l *xmlLoadImpl) processingInstruction(procInst xml.ProcInst) {
}

func (l *xmlLoadImpl) directive(directive string) {
}

type xmlStringSegment struct {
	strings.Builder
	lineWidth int
}

type xmlString struct {
	segments           []*xmlStringSegment
	currentSegment     *xmlStringSegment
	lineWidth          int
	depth              int
	indentation        string
	indents            []string
	lastElementIsStart bool
	elementNames       []string
}

const MaxInt = int(^uint(0) >> 1)

func newXmlString() *xmlString {
	segment := &xmlStringSegment{}
	s := &xmlString{
		segments:           []*xmlStringSegment{segment},
		currentSegment:     segment,
		lineWidth:          MaxInt,
		depth:              0,
		indentation:        "    ",
		indents:            []string{""},
		lastElementIsStart: false,
	}
	return s
}

func (s *xmlString) write(w io.Writer) {
	for _, segment := range s.segments {
		w.Write([]byte(segment.String()))
	}
}

func (s *xmlString) add(newString string) {
	if s.lineWidth != MaxInt {
		s.currentSegment.lineWidth += len(newString)
	}
	s.currentSegment.WriteString(newString)
}

func (s *xmlString) addLine() {
	s.add("\n")
	s.currentSegment.lineWidth = 0
}

func (s *xmlString) startElement(name string) {
	if s.lastElementIsStart {
		s.closeStartElement()
	}
	s.elementNames = append(s.elementNames, name)
	if len(name) > 0 {
		s.depth++
		s.add(s.getElementIndent())
		s.add("<")
		s.add(name)
		s.lastElementIsStart = true
	} else {
		s.add(s.getElementIndentWithExtra(1))
	}
}

func (s *xmlString) closeStartElement() {
	s.add(">")
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) endElement() {
	if s.lastElementIsStart {
		s.endEmptyElement()
	} else {
		name := s.removeLast()
		if name != "" {
			s.add(s.getElementIndentWithExtra(1))
			s.add("</")
			s.add(name)
			s.add(">")
			s.addLine()
		}
	}
}

func (s *xmlString) endEmptyElement() {
	s.removeLast()
	s.add("/>")
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) removeLast() string {
	end := len(s.elementNames) - 1
	result := s.elementNames[end]
	s.elementNames = s.elementNames[:end]
	if result != "" {
		s.depth--
	}
	return result
}

func (s *xmlString) addAttribute(name string, value string) {
	s.startAttribute(name)
	s.addAttributeContent(value)
	s.endAttribute()
}

func (s *xmlString) startAttribute(name string) {
	if s.currentSegment.lineWidth > s.lineWidth {
		s.addLine()
		s.add(s.getAttributeIndent())
	} else {
		s.add(" ")
	}
	s.add(name)
	s.add("=\"")
}

func (s *xmlString) addAttributeContent(content string) {
	s.add(content)
}

func (s *xmlString) endAttribute() {
	s.add("\"")
}

func (s *xmlString) addNil(name string) {
	if s.lastElementIsStart {
		s.closeStartElement()
	}

	s.depth++
	s.add(s.getElementIndent())
	s.add("<")
	s.add(name)
	if s.currentSegment.lineWidth > s.lineWidth {
		s.addLine()
		s.add(s.getAttributeIndent())
	} else {
		s.add(" ")
	}
	s.add("xsi:nil=\"true\"/>")
	s.depth--
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) addContent(name string, content string) {
	if s.lastElementIsStart {
		s.closeStartElement()
	}
	s.depth++
	s.add(s.getElementIndent())
	s.add("<")
	s.add(name)
	s.add(">")
	s.add(content)
	s.add("</")
	s.depth--
	s.add(name)
	s.add(">")
	s.addLine()
	s.lastElementIsStart = false
}

func (s *xmlString) getElementIndent() string {
	return s.getElementIndentWithExtra(0)
}

func (s *xmlString) getElementIndentWithExtra(extra int) string {
	nesting := s.depth + extra - 1
	for i := len(s.indents) - 1; i < nesting; i++ {
		s.indents = append(s.indents, s.indents[i]+"  ")
	}
	return s.indents[nesting]
}

func (s *xmlString) getAttributeIndent() string {
	nesting := s.depth + 1
	for i := len(s.indents) - 1; i < nesting; i++ {
		s.indents = append(s.indents, s.indents[i]+"  ")
	}
	return s.indents[nesting]
}

func (s *xmlString) mark() *xmlStringSegment {
	r := s.currentSegment
	s.currentSegment = &xmlStringSegment{}
	s.segments = append(s.segments, s.currentSegment)
	return r
}

func (s *xmlString) resetToMark(segment *xmlStringSegment) {
	if segment != nil {
		s.currentSegment = segment
	}
}

const (
	transient                              = iota
	datatype_single                        = iota
	datatype_element_single                = iota
	datatype_content_single                = iota
	datatype_single_nillable               = iota
	datatype_many                          = iota
	object_contain_single                  = iota
	object_contain_many                    = iota
	object_href_single                     = iota
	object_href_many                       = iota
	object_contain_single_unsettable       = iota
	object_contain_many_unsettable         = iota
	object_href_single_unsettable          = iota
	object_href_many_unsettable            = iota
	object_element_single                  = iota
	object_element_single_unsettable       = iota
	object_element_many                    = iota
	object_element_idref_single            = iota
	object_element_idref_single_unsettable = iota
	object_element_idref_many              = iota
	attribute_feature_map                  = iota
	element_feature_map                    = iota
	object_attribute_single                = iota
	object_attribute_many                  = iota
	object_attribute_idref_single          = iota
	object_attribute_idref_many            = iota
	datatype_attribute_many                = iota
)

type xmlSaveImpl struct {
	interfaces    interface{}
	resource      XMLResource
	str           *xmlString
	packages      map[EPackage]string
	uriToPrefixes map[string][]string
	prefixesToURI map[string]string
	featureKinds  map[EStructuralFeature]int
	namespaces    *xmlNamespaces
	keepDefaults  bool
}

func newXMLSaveImpl() *xmlSaveImpl {
	s := new(xmlSaveImpl)
	s.interfaces = s
	s.str = newXmlString()
	s.packages = make(map[EPackage]string)
	s.uriToPrefixes = make(map[string][]string)
	s.prefixesToURI = make(map[string]string)
	s.featureKinds = make(map[EStructuralFeature]int)
	s.namespaces = newXmlNamespaces()
	return s
}

func (s *xmlSaveImpl) save(resource XMLResource, w io.Writer) {
	s.resource = resource
	c := s.resource.GetContents()
	if c.Empty() {
		return
	}

	// header
	s.saveHeader()

	// top object
	object := c.Get(0).(EObject)
	mark := s.saveTopObject(object)

	// namespaces
	s.str.resetToMark(mark)
	s.interfaces.(xmlSaveInternal).saveNamespaces()

	// write result
	s.str.write(w)
}

func (s *xmlSaveImpl) saveHeader() {
	s.str.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
	s.str.addLine()
}

func (s *xmlSaveImpl) saveTopObject(eObject EObject) *xmlStringSegment {
	eClass := eObject.EClass()
	name := s.getQName(eClass)
	s.str.startElement(name)
	mark := s.str.mark()
	s.saveElementID(eObject)
	s.saveFeatures(eObject, false)
	return mark
}

func (s *xmlSaveImpl) saveNamespaces() {
	var prefixes []string
	for prefix, _ := range s.prefixesToURI {
		prefixes = append(prefixes, prefix)
	}
	sort.Strings(prefixes)
	for _, prefix := range prefixes {
		s.str.addAttribute("xmlns:"+prefix, s.prefixesToURI[prefix])
	}
}

func (s *xmlSaveImpl) saveElementID(eObject EObject) {
}

func (s *xmlSaveImpl) saveFeatures(eObject EObject, attributesOnly bool) bool {
	eClass := eObject.EClass()
	eAllFeatures := eClass.GetEAllStructuralFeatures()
	var elementFeatures []int
	elementCount := 0
	i := 0
	for it := eAllFeatures.Iterator(); it.HasNext(); i++ {
		// current feature
		eFeature := it.Next().(EStructuralFeature)
		// compute feature kind
		kind, ok := s.featureKinds[eFeature]
		if !ok {
			kind = s.getSaveFeatureKind(eFeature)
			s.featureKinds[eFeature] = kind
		}

		if kind != transient && s.shouldSaveFeature(eObject, eFeature) {
			switch kind {
			case datatype_single:
				s.saveDataTypeSingle(eObject, eFeature)
				continue
			case datatype_single_nillable:
				if !s.isNil(eObject, eFeature) {
					s.saveDataTypeSingle(eObject, eFeature)
					continue
				}
			case object_contain_many_unsettable:
				fallthrough
			case datatype_many:
				if s.isEmpty(eObject, eFeature) {
					s.saveManyEmpty(eObject, eFeature)
					continue
				}
			case object_contain_single_unsettable:
			case object_contain_single:
			case object_contain_many:
			case object_href_single_unsettable:
				if !s.isNil(eObject, eFeature) {
					switch s.getSaveResourceKindSingle(eObject, eFeature) {
					case cross:
					case same:
						s.saveIDRefSingle(eObject, eFeature)
						continue
					default:
						continue
					}
				}
			case object_href_single:
				switch s.getSaveResourceKindSingle(eObject, eFeature) {
				case cross:
				case same:
					s.saveIDRefSingle(eObject, eFeature)
					continue
				default:
					continue
				}
			case object_href_many_unsettable:
				if s.isEmpty(eObject, eFeature) {
					s.saveManyEmpty(eObject, eFeature)
					continue
				} else {
					switch s.getSaveResourceKindMany(eObject, eFeature) {
					case cross:
					case same:
						s.saveIDRefMany(eObject, eFeature)
						continue
					default:
						continue
					}
				}

			case object_href_many:
				switch s.getSaveResourceKindMany(eObject, eFeature) {
				case cross:
				case same:
					s.saveIDRefMany(eObject, eFeature)
					continue
				default:
					continue
				}
			default:
				continue
			}
			if attributesOnly {
				continue
			}
			if elementFeatures == nil {
				elementFeatures = make([]int, eAllFeatures.Size())
			}
			elementFeatures[elementCount] = i
			elementCount++
		}
	}
	if elementFeatures == nil {
		s.str.endEmptyElement()
		return false
	}
	for i := 0; i < elementCount; i++ {
		eFeature := eAllFeatures.Get(elementFeatures[i]).(EStructuralFeature)
		kind := s.featureKinds[eFeature]
		switch kind {
		case datatype_single_nillable:
			s.saveNil(eObject, eFeature)
		case datatype_many:
			s.saveDataTypeMany(eObject, eFeature)
		case object_contain_single_unsettable:
			if s.isNil(eObject, eFeature) {
				s.saveNil(eObject, eFeature)
			} else {
				s.saveContainedSingle(eObject, eFeature)
			}
		case object_contain_single:
			s.saveContainedSingle(eObject, eFeature)
		case object_contain_many_unsettable:
			fallthrough
		case object_contain_many:
			s.saveContainedMany(eObject, eFeature)
		case object_href_single_unsettable:
			if s.isNil(eObject, eFeature) {
				s.saveNil(eObject, eFeature)
			} else {
				s.saveHRefSingle(eObject, eFeature)
			}
		case object_href_single:
			s.saveHRefSingle(eObject, eFeature)
		case object_href_many_unsettable:
			fallthrough
		case object_href_many:
			s.saveHRefMany(eObject, eFeature)
		}
	}

	s.str.endElement()
	return true
}

func (s *xmlSaveImpl) saveDataTypeSingle(eObject EObject, eFeature EStructuralFeature) {
	val := eObject.EGet(eFeature)
	str, ok := s.getDataType(val, eFeature, true)
	if ok {
		s.str.addAttribute(s.getFeatureQName(eFeature), str)
	}
}

func (s *xmlSaveImpl) saveDataTypeMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	d := eFeature.GetEType().(EDataType)
	p := d.GetEPackage()
	f := p.GetEFactoryInstance()
	name := s.getFeatureQName(eFeature)
	for it := l.Iterator(); it.HasNext(); {
		value := it.Next()
		if value == nil {
			s.str.startElement(name)
			s.str.addAttribute("xsi:nil", "true")
			s.str.endEmptyElement()
			s.uriToPrefixes[xsiURI] = []string{xsiNS}
			s.prefixesToURI[xsiNS] = xsiURI
		} else {
			str := f.ConvertToString(d, value)
			s.str.addContent(name, str)
		}
	}
}

func (s *xmlSaveImpl) saveManyEmpty(eObject EObject, eFeature EStructuralFeature) {
	s.str.addAttribute(s.getFeatureQName(eFeature), "")
}

func (s *xmlSaveImpl) saveEObjectSingle(eObject EObject, eFeature EStructuralFeature) {
	value, _ := eObject.EGet(eFeature).(EObject)
	if value != nil {
		id := s.getHRef(value)
		s.str.addAttribute(s.getFeatureQName(eFeature), id)
	}
}

func (s *xmlSaveImpl) saveEObjectMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	failure := false
	var buffer strings.Builder
	for it := l.Iterator(); ; {
		value, _ := it.Next().(EObject)
		if value != nil {
			id := s.getHRef(value)
			if id == "" {
				failure = true
				if !it.HasNext() {
					break
				}
			} else {
				buffer.WriteString(id)
				if it.HasNext() {
					buffer.WriteString(" ")
				} else {
					break
				}
			}
		}
	}
	if !failure && buffer.Len() > 0 {
		s.str.addAttribute(s.getFeatureQName(eFeature), buffer.String())
	}
}

func (s *xmlSaveImpl) saveNil(eObject EObject, eFeature EStructuralFeature) {
	s.str.addNil(s.getFeatureQName(eFeature))
}

func (s *xmlSaveImpl) saveContainedSingle(eObject EObject, eFeature EStructuralFeature) {
	value, _ := eObject.EGet(eFeature).(EObjectInternal)
	if value != nil {
		s.saveEObjectInternal(value, eFeature)
	}
}

func (s *xmlSaveImpl) saveContainedMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	for it := l.Iterator(); it.HasNext(); {
		value, _ := it.Next().(EObjectInternal)
		if value != nil {
			s.saveEObjectInternal(value, eFeature)
		}
	}
}

func (s *xmlSaveImpl) saveEObjectInternal(o EObjectInternal, f EStructuralFeature) {
	if o.EDirectResource() != nil || o.EIsProxy() {
		s.saveHRef(o, f)
	} else {
		s.saveEObject(o, f)
	}
}

func (s *xmlSaveImpl) saveEObject(o EObject, f EStructuralFeature) {
	s.str.startElement(s.getFeatureQName(f))
	eClass := o.EClass()
	eType := f.GetEType()
	if eType != eClass && eType != GetPackage().GetEObject() {
		s.saveTypeAttribute(eClass)
	}
	s.saveElementID(o)
	s.saveFeatures(o, false)
}

func (s *xmlSaveImpl) saveTypeAttribute(eClass EClass) {
	s.str.addAttribute("xsi:type", s.getQName(eClass))
	s.uriToPrefixes[xsiURI] = []string{xsiNS}
	s.prefixesToURI[xsiNS] = xsiURI
}

func (s *xmlSaveImpl) saveHRefSingle(eObject EObject, eFeature EStructuralFeature) {
	value, _ := eObject.EGet(eFeature).(EObject)
	if value != nil {
		s.saveHRef(value, eFeature)
	}
}

func (s *xmlSaveImpl) saveHRefMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	for it := l.Iterator(); it.HasNext(); {
		value, _ := it.Next().(EObject)
		if value != nil {
			s.saveHRef(value, eFeature)
		}
	}
}

func (s *xmlSaveImpl) saveHRef(eObject EObject, eFeature EStructuralFeature) {
	href := s.getHRef(eObject)
	if href != "" {
		s.str.startElement(s.getFeatureQName(eFeature))
		eClass := eObject.EClass()
		eType, _ := eFeature.GetEType().(EClass)
		if eType != eClass && eType != nil && eType.IsAbstract() {
			s.saveTypeAttribute(eClass)
		}
		s.str.addAttribute("href", href)
		s.str.endEmptyElement()
	}
}

func (s *xmlSaveImpl) saveIDRefSingle(eObject EObject, eFeature EStructuralFeature) {
	value, _ := eObject.EGet(eFeature).(EObject)
	if value != nil {
		id := s.getIDRef(value)
		if id != "" {
			s.str.addAttribute(s.getFeatureQName(eFeature), id)
		}
	}
}

func (s *xmlSaveImpl) saveIDRefMany(eObject EObject, eFeature EStructuralFeature) {
	l := eObject.EGet(eFeature).(EList)
	failure := false
	var buffer strings.Builder
	for it := l.Iterator(); ; {
		value, _ := it.Next().(EObject)
		if value != nil {
			id := s.getIDRef(value)
			if id == "" {
				failure = true
				if !it.HasNext() {
					break
				}
			} else {
				buffer.WriteString(id)
				if it.HasNext() {
					buffer.WriteString(" ")
				} else {
					break
				}
			}
		}
	}
	if !failure && buffer.Len() > 0 {
		s.str.addAttribute(s.getFeatureQName(eFeature), buffer.String())
	}
}

func (s *xmlSaveImpl) isNil(eObject EObject, eFeature EStructuralFeature) bool {
	return eObject.EGet(eFeature) == nil
}

func (s *xmlSaveImpl) isEmpty(eObject EObject, eFeature EStructuralFeature) bool {
	return eObject.EGet(eFeature).(EList).Empty()
}

func (s *xmlSaveImpl) shouldSaveFeature(o EObject, f EStructuralFeature) bool {
	return o.EIsSet(f) || (s.keepDefaults && f.GetDefaultValueLiteral() != "")
}

func (s *xmlSaveImpl) getSaveFeatureKind(f EStructuralFeature) int {
	if f.IsTransient() {
		return transient
	}

	isMany := f.IsMany()
	isUnsettable := f.IsUnsettable()

	if r, _ := f.(EReference); r != nil {
		if r.IsContainment() {
			if isMany {
				if isUnsettable {
					return object_contain_many_unsettable
				} else {
					return object_contain_many
				}
			} else {
				if isUnsettable {
					return object_contain_single_unsettable
				} else {
					return object_contain_single
				}
			}
		}
		opposite := r.GetEOpposite()
		if opposite != nil && opposite.IsContainment() {
			return transient
		}
		if isMany {
			if isUnsettable {
				return object_href_many_unsettable
			} else {
				return object_href_many
			}
		} else {
			if isUnsettable {
				return object_href_single_unsettable
			} else {
				return object_href_single
			}
		}
	} else {
		// Attribute
		d, _ := f.GetEType().(EDataType)
		if !d.IsSerializable() {
			return transient
		}
		if isMany {
			return datatype_many
		}
		if isUnsettable {
			return datatype_single_nillable
		}
		return datatype_single

	}

}

const (
	skip  = iota
	same  = iota
	cross = iota
)

func (s *xmlSaveImpl) getSaveResourceKindSingle(eObject EObject, eFeature EStructuralFeature) int {
	value, _ := eObject.EGet(eFeature).(EObjectInternal)
	if value == nil {
		return skip
	} else if value.EIsProxy() {
		return cross
	} else {
		resource := value.EResource()
		if resource == s.resource || resource == nil {
			return same
		}
		return cross
	}
}

func (s *xmlSaveImpl) getSaveResourceKindMany(eObject EObject, eFeature EStructuralFeature) int {
	list, _ := eObject.EGet(eFeature).(EList)
	if list == nil || list.Empty() {
		return skip
	}
	for it := list.Iterator(); it.HasNext(); {
		o, _ := it.Next().(EObjectInternal)
		if o == nil {
			return skip
		} else if o.EIsProxy() {
			return cross
		} else {
			r := o.EResource()
			if r != nil && r != s.resource {
				return cross
			}
		}

	}
	return same
}

func (s *xmlSaveImpl) getQName(eClass EClass) string {
	return s.getElementQName(eClass.GetEPackage(), eClass.GetName(), false)
}

func (s *xmlSaveImpl) getElementQName(ePackage EPackage, name string, mustHavePrefix bool) string {
	nsPrefix := s.getPrefix(ePackage, mustHavePrefix)
	if nsPrefix == "" {
		return name
	} else if len(name) == 0 {
		return nsPrefix
	} else {
		return nsPrefix + ":" + name
	}
}

func (s *xmlSaveImpl) getFeatureQName(eFeature EStructuralFeature) string {
	return eFeature.GetName()
}

func (s *xmlSaveImpl) getPrefix(ePackage EPackage, mustHavePrefix bool) string {
	nsPrefix, ok := s.packages[ePackage]
	if !ok {
		nsURI := ePackage.GetNsURI()
		found := false
		prefixes := s.uriToPrefixes[nsURI]
		if prefixes != nil {
			for _, prefix := range prefixes {
				nsPrefix = prefix
				if !mustHavePrefix || len(nsPrefix) > 0 {
					found = true
					break
				}
			}
		}
		if !found {
			nsPrefix, ok = s.namespaces.getPrefix(nsURI)
			if ok {
				return nsPrefix
			}
			nsPrefix = ePackage.GetNsPrefix()
			if len(nsPrefix) == 0 && mustHavePrefix {
				nsPrefix = "_"
			}

			if uri, exists := s.prefixesToURI[nsPrefix]; exists && uri != nsURI {
				index := 1
				for _, exists = s.prefixesToURI[nsPrefix+"_"+fmt.Sprintf("%d", index)]; exists; {
					index++
				}
				nsPrefix += "_" + fmt.Sprintf("%d", index)
			}
			s.prefixesToURI[nsPrefix] = nsURI
		}

		s.packages[ePackage] = nsPrefix
	}
	return nsPrefix
}

func (s *xmlSaveImpl) getDataType(value interface{}, f EStructuralFeature, isAttribute bool) (string, bool) {
	if value == nil {
		return "", false
	} else {
		d := f.GetEType().(EDataType)
		p := d.GetEPackage()
		f := p.GetEFactoryInstance()
		s := f.ConvertToString(d, value)
		return s, true
	}
}

func (s *xmlSaveImpl) getHRef(eObject EObject) string {
	eInternal, _ := eObject.(EObjectInternal)
	if eInternal != nil {
		uri := eInternal.EProxyURI()
		if uri == nil {
			eResource := eObject.EResource()
			if eResource == nil {
				return ""
			} else {
				return s.getResourceHRef(eResource, eObject)
			}
		} else {
			return uri.String()
		}
	}
	return ""
}

func (s *xmlSaveImpl) getResourceHRef(resource EResource, object EObject) string {
	uri := resource.GetURI()
	uri.Fragment = resource.GetURIFragment(object)
	return uri.String()
}

func (s *xmlSaveImpl) getIDRef(eObject EObject) string {
	if s.resource == nil {
		return ""
	} else {
		return "#" + s.resource.GetURIFragment(eObject)
	}
}

type xmlResourceImpl struct {
	*EResourceImpl
}

func newXMLResourceImpl() *xmlResourceImpl {
	r := new(xmlResourceImpl)
	r.EResourceImpl = NewEResourceImpl()
	r.SetInterfaces(r)
	return r
}

func (r *xmlResourceImpl) DoLoad(rd io.Reader) {
	resource := r.GetInterfaces().(XMLResource)
	l := resource.createLoad()
	l.load(resource, rd)
}

func (r *xmlResourceImpl) DoSave(w io.Writer) {
	resource := r.GetInterfaces().(XMLResource)
	s := resource.createSave()
	s.save(resource, w)
}

func (r *xmlResourceImpl) createLoad() xmlLoad {
	return newXMLLoadImpl()
}

func (r *xmlResourceImpl) createSave() xmlSave {
	return newXMLSaveImpl()
}
