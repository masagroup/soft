package ecore

import (
	"net/url"
	"strconv"
	"strings"
)

type resourceNotification struct {
	*abstractNotification
	notifier  ENotifier
	featureID int
}

func (n *resourceNotification) GetNotifier() ENotifier {
	return n.notifier
}

func (n *resourceNotification) GetFeature() EStructuralFeature {
	return nil
}

func (n *resourceNotification) GetFeatureID() int {
	return n.featureID
}

func newResourceNotification(notifier ENotifier, featureID int, eventType EventType, oldValue interface{}, newValue interface{}, position int) *resourceNotification {
	n := new(resourceNotification)
	n.abstractNotification = NewAbstractNotification(eventType, oldValue, newValue, position)
	n.notifier = notifier
	n.featureID = featureID
	return n
}

type resourceContents struct {
	*ENotifyingListImpl
	resource EResource
}

func newResourceContents(resource EResource) *resourceContents {
	rc := new(resourceContents)
	rc.ENotifyingListImpl = NewENotifyingListImpl()
	rc.resource = resource
	rc.interfaces = rc
	return rc
}

func (rc *resourceContents) GetNotifier() ENotifier {
	return rc.resource
}

func (rc *resourceContents) GetFeatureID() int {
	return RESOURCE__CONTENTS
}

func (rc *resourceContents) inverseAdd(object interface{}, notifications ENotificationChain) ENotificationChain {
	eObject := object.(EObjectInternal)
	n := notifications
	n = eObject.ESetResource(rc.resource, n)
	rc.resource.Attached(eObject)
	return n
}

func (rc *resourceContents) inverseRemove(object interface{}, notifications ENotificationChain) ENotificationChain {
	eObject := object.(EObjectInternal)
	rc.resource.Detached(eObject)
	n := notifications
	n = eObject.ESetResource(nil, n)
	return n
}

//EResource ...
type EResourceImpl struct {
	*Notifier
	resourceSet EResourceSet
	uri         *url.URL
	contents    EList
}

// NewBasicEObject is BasicEObject constructor
func NewEResourceImpl() *EResourceImpl {
	r := new(EResourceImpl)
	r.Notifier = NewNotifier()
	r.SetInterfaces(r)
	return r
}

func (r *EResourceImpl) GetResourceSet() EResourceSet {
	return r.resourceSet
}

func (r *EResourceImpl) GetURI() *url.URL {
	return r.uri
}

func (r *EResourceImpl) SetURI(uri *url.URL) {
	oldURI := r.uri
	r.uri = uri
	if r.ENotificationRequired() {
		r.ENotify(newResourceNotification(r, RESOURCE__URI, SET, oldURI, uri, -1))
	}
}

func (r *EResourceImpl) GetContents() EList {
	if r.contents == nil {
		r.contents = newResourceContents(r)
	}
	return r.contents
}

func (r *EResourceImpl) GetAllContents() EIterator {
	return newTreeIterator(r, false, func(o interface{}) EIterator {
		if o == r {
			return o.(EResource).GetContents().Iterator()
		}
		return o.(EObject).EContents().Iterator()
	})
}

func (r *EResourceImpl) GetEObject(uriFragment string) EObject {
	id := uriFragment
	size := len(uriFragment)
	if size > 0 {
		if uriFragment[0] == '/' {
			path := strings.Split(uriFragment, "/")
			return r.getObjectByPath(path)
		} else if uriFragment[size-1] == '?' {
			if index := strings.LastIndex(uriFragment[:size-2], "?"); index != -1 {
				id = uriFragment[:index]
			}
		}
	}
	return r.getObjectByID(id)
}

func (r *EResourceImpl) GetURIFragment(eObject EObject) string {
	id := GetEObjectID(eObject)
	if len(id) == 0 {
		internalEObject := eObject.(EObjectInternal)
		if internalEObject.EDirectResource() == r.interfaces {
			return "/" + r.getURIFragmentRootSegment(eObject)
		} else {
			fragmentPath := []string{}
			isContained := false
			for eContainer := eObject.EContainer(); eContainer != nil; eContainer = eObject.EContainer() {
				internalEContainer := eContainer.(EObjectInternal)
				if len(id) == 0 {
					fragmentPath = append([]string{internalEContainer.EURIFragmentSegment(internalEObject.EContainingFeature(), internalEObject)}, fragmentPath...)
				}
				internalEObject = eContainer.(EObjectInternal)
				if internalEContainer.EDirectResource() == r.interfaces {
					isContained = true
					break
				}
			}
			if !isContained {
				fragmentPath = append([]string{"/-1"}, fragmentPath...)
			}
			if len(id) == 0 {
				fragmentPath = append([]string{r.getURIFragmentRootSegment(internalEObject)}, fragmentPath...)
			} else {
				fragmentPath = append([]string{"?" + id}, fragmentPath...)
			}
			fragmentPath = append([]string{""}, fragmentPath...)
			return strings.Join(fragmentPath, "/")
		}
	}
	return id
}

func (r *EResourceImpl) getURIFragmentRootSegment(eObject EObject) string {
	contents := r.GetContents()
	if contents.Empty() {
		return ""
	} else {
		return strconv.Itoa(contents.IndexOf(eObject))
	}
}

func (r *EResourceImpl) getObjectByID(id string) EObject {
	for it := r.GetAllContents(); it.HasNext(); {
		eObject := it.Next().(EObject)
		objectID := GetEObjectID(eObject)
		if id == objectID {
			return eObject
		}
	}
	return nil
}

func (r *EResourceImpl) getObjectByPath(uriFragmentPath []string) EObject {
	var eObject EObject
	if uriFragmentPath == nil || len(uriFragmentPath) == 0 {
		eObject = r.getObjectForRootSegment("")
	} else {
		eObject = r.getObjectForRootSegment(uriFragmentPath[0])
	}
	for i := 1; i < len(uriFragmentPath) && eObject != nil; i++ {
		eObject = eObject.(EObjectInternal).EObjectForFragmentSegment(uriFragmentPath[i])
	}
	return eObject
}

func (r *EResourceImpl) getObjectForRootSegment(rootSegment string) EObject {
	position := 0
	if len(rootSegment) > 0 {
		if rootSegment[0] == '?' {
			return r.getObjectByID(rootSegment[1:])
		} else {
			position, _ = strconv.Atoi(rootSegment)
		}
	}
	if position >= 0 && position < r.GetContents().Size() {
		return r.GetContents().Get(position).(EObject)
	}
	return nil
}

func (r *EResourceImpl) Attached(object EObject) {

}

func (r *EResourceImpl) Detached(object EObject) {

}
