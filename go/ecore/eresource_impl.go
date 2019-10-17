package ecore

import "net/url"

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
	return r
}

func (r *EResourceImpl) GetResourceSet() EResourceSet {
	return r.resourceSet
}

func (r *EResourceImpl) GetURI() *url.URL {
	return r.uri
}

func (r *EResourceImpl) SetURI(uri *url.URL) {
	r.uri = uri
}

func (r *EResourceImpl) GetContents() EList {
	if r.contents == nil {
		r.contents = newResourceContents(r)
	}
	return r.contents
}

func (r *EResourceImpl) GetEObject(uriFragment string) EObject {
	return nil
}

func (r *EResourceImpl) GetURIFragment(EObject) string {
	return "nil"
}

func (r *EResourceImpl) Attached(object EObject) {

}

func (r *EResourceImpl) Detached(object EObject) {

}
