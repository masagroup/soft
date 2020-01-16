// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

import (
	"net/url"
	"strconv"
	"strings"
	"unicode"
	"unicode/utf8"
)

// BasicEObject is a basic implementation of an EObject
type BasicEObject struct {
	*Notifier
	resource           EResource
	container          EObject
	containerFeatureID int
	proxyURI           *url.URL
}

// EObjectInternal ...
type EObjectInternal interface {
	EObject

	EStaticClass() EClass

	EDirectResource() EResource
	ESetResource(resource EResource, notifications ENotificationChain) ENotificationChain
	EInverseAdd(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain
	EInverseRemove(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain

	EGetFromID(featureID int, resolve bool, core bool) interface{}
	ESetFromID(featureID int, newValue interface{})
	EUnsetFromID(featureID int)
	EIsSetFromID(featureID int) bool
	EInvokeFromID(operationID int, arguments EList) interface{}

	EBasicInverseAdd(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain
	EBasicInverseRemove(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain

	EObjectForFragmentSegment(string) EObject
	EURIFragmentSegment(EStructuralFeature, EObject) string

	EProxyURI() *url.URL
	ESetProxyURI(uri *url.URL)
	EResolveProxy(proxy EObject) EObject
}

// NewBasicEObject is BasicEObject constructor
func NewBasicEObject() *BasicEObject {
	o := new(BasicEObject)
	o.Notifier = NewNotifier()
	o.interfaces = o
	o.containerFeatureID = -1
	return o
}

// AsEObject ...
func (o *BasicEObject) AsEObject() EObject {
	return o.interfaces.(EObject)
}

// AsEObjectInternal ...
func (o *BasicEObject) AsEObjectInternal() EObjectInternal {
	return o.interfaces.(EObjectInternal)
}

// EClass ...
func (o *BasicEObject) EClass() EClass {
	return o.AsEObjectInternal().EStaticClass()
}

// EIsProxy ...
func (o *BasicEObject) EIsProxy() bool {
	return o.proxyURI != nil
}

// EResource ...
func (o *BasicEObject) EResource() EResource {
	resource := o.resource
	if resource == nil {
		if o.container != nil {
			resource = o.container.EResource()
		}
	}
	return resource
}

// EDirectResource ...
func (o *BasicEObject) EDirectResource() EResource {
	return o.resource
}

func (o *BasicEObject) ESetDirectResource(resource EResource) {
	o.resource = resource
}

// ESetResource ...
func (o *BasicEObject) ESetResource(newResource EResource, n ENotificationChain) ENotificationChain {
	notifications := n
	oldResource := o.EDirectResource()
	if oldResource != nil && newResource != nil {
		list := oldResource.GetContents().(ENotifyingList)
		notifications = list.RemoveWithNotification(o.AsEObject(), notifications)
		oldResource.Detached(o.AsEObject())
	}

	eContainer := o.container
	if eContainer != nil {
		if o.EContainmentFeature().IsResolveProxies() {
			oldContainerResource := eContainer.EResource()
			if oldContainerResource != nil {
				if newResource == nil {
					// If we're not setting a new resource, attach it to the old container's resource.
					oldContainerResource.Attached(o.AsEObject())
				} else if oldResource == nil {
					// If we didn't detach it from an old resource already, detach it from the old container's resource.
					oldContainerResource.Detached(o.AsEObject())
				}
			}
		} else {
			notifications = o.EBasicRemoveFromContainer(notifications)
			notifications = o.EBasicSetContainer(nil, -1, notifications)
		}
	}
	o.ESetDirectResource(newResource)
	return notifications
}

// EContainer ...
func (o *BasicEObject) EContainer() EObject {
	return o.container
}

// EContainerFeatureID ...
func (o *BasicEObject) EContainerFeatureID() int {
	return o.containerFeatureID
}

// EContainingFeature ...
func (o *BasicEObject) EContainingFeature() EStructuralFeature {
	if o.container != nil {
		if o.containerFeatureID <= EOPPOSITE_FEATURE_BASE {
			return o.container.EClass().GetEStructuralFeature(EOPPOSITE_FEATURE_BASE - o.containerFeatureID)
		} else {
			return o.AsEObject().EClass().GetEStructuralFeature(o.containerFeatureID).(EReference).GetEOpposite()
		}
	}
	return nil
}

// EContainmentFeature ...
func (o *BasicEObject) EContainmentFeature() EReference {
	return eContainmentFeature(o, o.container, o.containerFeatureID)
}

func eContainmentFeature(o EObject, container EObject, containerFeatureID int) EReference {
	if container != nil {
		if containerFeatureID <= EOPPOSITE_FEATURE_BASE {
			feature := container.EClass().GetEStructuralFeature(EOPPOSITE_FEATURE_BASE - containerFeatureID)
			reference, isReference := feature.(EReference)
			if isReference {
				return reference
			}
		} else {
			feature := o.EClass().GetEStructuralFeature(containerFeatureID)
			reference, isReference := feature.(EReference)
			if isReference {
				return reference
			}
		}
		panic("The containment feature could not be located")
	}
	return nil
}

// EContents ...
func (o *BasicEObject) EContents() EList {
	return o.eContentsList(o.EClass().GetEContainments())
}

// ECrossReferences ...
func (o *BasicEObject) ECrossReferences() EList {
	return o.eContentsList(o.EClass().GetECrossReferences())
}

func (o *BasicEObject) eContentsList(refs EList) EList {
	data := []interface{}{}
	for it := refs.Iterator(); it.HasNext(); {
		ref := it.Next().(EStructuralFeature)
		if o.EIsSet(ref) {
			value := o.EGet(ref)
			if ref.IsMany() {
				l := value.(EList)
				data = append(data, l.ToArray()...)
			} else if value != nil {
				data = append(data, value)
			}
		}
	}
	return NewImmutableEList(data)
}

// EAllContents ...
func (o *BasicEObject) EAllContents() EIterator {
	return newEAllContentsIterator(o)
}

func (o *BasicEObject) eDerivedStructuralFeatureID(feature EStructuralFeature) int {
	if !o.AsEObject().EClass().GetEAllStructuralFeatures().Contains(feature) {
		panic("The feature '" + feature.GetName() + "' is not a valid feature")
	}
	return feature.GetFeatureID()
}

func (o *BasicEObject) eDerivedOperationID(operation EOperation) int {
	if !o.AsEObject().EClass().GetEAllOperations().Contains(operation) {
		panic("The operation '" + operation.GetName() + "' is not a valid operation")
	}
	return operation.GetOperationID()
}

// EGet ...
func (o *BasicEObject) EGet(feature EStructuralFeature) interface{} {
	return o.eGetFromFeature(feature, true, true)
}

// EGetResolve ...
func (o *BasicEObject) EGetResolve(feature EStructuralFeature, resolve bool) interface{} {
	return o.eGetFromFeature(feature, resolve, true)
}

func (o *BasicEObject) eGetFromFeature(feature EStructuralFeature, resolve bool, core bool) interface{} {
	featureID := o.eDerivedStructuralFeatureID(feature)
	if featureID >= 0 {
		return o.AsEObjectInternal().EGetFromID(featureID, resolve, core)
	}
	panic("The feature '" + feature.GetName() + "' is not a valid feature")
}

// EGetFromID ...
func (o *BasicEObject) EGetFromID(featureID int, resolve bool, core bool) interface{} {
	feature := o.AsEObject().EClass().GetEStructuralFeature(featureID)
	if feature == nil {
		panic("Invalid featureID: " + strconv.Itoa(featureID))
	}
	return nil
}

// ESet ...
func (o *BasicEObject) ESet(feature EStructuralFeature, newValue interface{}) {
	featureID := o.eDerivedStructuralFeatureID(feature)
	if featureID >= 0 {
		o.AsEObjectInternal().ESetFromID(featureID, newValue)
	} else {
		panic("The feature '" + feature.GetName() + "' is not a valid feature")
	}
}

// ESetFromID ...
func (o *BasicEObject) ESetFromID(featureID int, newValue interface{}) {
	feature := o.AsEObject().EClass().GetEStructuralFeature(featureID)
	if feature == nil {
		panic("Invalid featureID: " + strconv.Itoa(featureID))
	}
}

// EIsSet ...
func (o *BasicEObject) EIsSet(feature EStructuralFeature) bool {
	featureID := o.eDerivedStructuralFeatureID(feature)
	if featureID >= 0 {
		return o.AsEObjectInternal().EIsSetFromID(featureID)
	}
	panic("The feature '" + feature.GetName() + "' is not a valid feature")
}

// EIsSetFromID ...
func (o *BasicEObject) EIsSetFromID(featureID int) bool {
	feature := o.AsEObject().EClass().GetEStructuralFeature(featureID)
	if feature == nil {
		panic("Invalid featureID: " + strconv.Itoa(featureID))
	}
	return false
}

// EUnset ...
func (o *BasicEObject) EUnset(feature EStructuralFeature) {
	featureID := o.eDerivedStructuralFeatureID(feature)
	if featureID >= 0 {
		o.AsEObjectInternal().EUnsetFromID(featureID)
	} else {
		panic("The feature '" + feature.GetName() + "' is not a valid feature")
	}
}

// EUnsetFromID ...
func (o *BasicEObject) EUnsetFromID(featureID int) {
	feature := o.AsEObject().EClass().GetEStructuralFeature(featureID)
	if feature == nil {
		panic("Invalid featureID: " + strconv.Itoa(featureID))
	}
}

// EInvoke ...
func (o *BasicEObject) EInvoke(operation EOperation, arguments EList) interface{} {
	operationID := o.eDerivedOperationID(operation)
	if operationID >= 0 {
		return o.AsEObjectInternal().EInvokeFromID(operationID, arguments)
	}
	panic("The operation '" + operation.GetName() + "' is not a valid operation")
}

// EInvokeFromID ...
func (o *BasicEObject) EInvokeFromID(operationID int, arguments EList) interface{} {
	operation := o.AsEObject().EClass().GetEOperation(operationID)
	if operation == nil {
		panic("Invalid operationID: " + strconv.Itoa(operationID))
	}
	return nil
}

// EStaticClass ...
func (o *BasicEObject) EStaticClass() EClass {
	return GetPackage().GetEObject()
}

// EInverseAdd ...
func (o *BasicEObject) EInverseAdd(otherEnd EObject, featureID int, n ENotificationChain) ENotificationChain {
	notifications := n
	if featureID >= 0 {
		return o.AsEObjectInternal().EBasicInverseAdd(otherEnd, featureID, notifications)
	} else {
		notifications = o.EBasicRemoveFromContainer(notifications)
		return o.EBasicSetContainer(otherEnd, featureID, notifications)
	}
}

// EInverseRemove ...
func (o *BasicEObject) EInverseRemove(otherEnd EObject, featureID int, n ENotificationChain) ENotificationChain {
	if featureID >= 0 {
		return o.AsEObjectInternal().EBasicInverseRemove(otherEnd, featureID, n)
	} else {
		return o.EBasicSetContainer(nil, featureID, n)
	}
}

// EProxyURI ...
func (o *BasicEObject) EProxyURI() *url.URL {
	return o.proxyURI
}

// ESetProxyURI ...
func (o *BasicEObject) ESetProxyURI(uri *url.URL) {
	o.proxyURI = uri
}

// EResolveProxy ...
func (o *BasicEObject) EResolveProxy(proxy EObject) EObject {
	return ResolveInObject(proxy, o.GetInterfaces().(EObject))
}

// EBasicInverseAdd ...
func (o *BasicEObject) EBasicInverseAdd(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	return notifications
}

// EBasicInverseRemove ...
func (o *BasicEObject) EBasicInverseRemove(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	return notifications
}

// EBasicSetContainer ...
func (o *BasicEObject) EBasicSetContainer(newContainer EObject, newContainerFeatureID int, n ENotificationChain) ENotificationChain {
	notifications := n
	oldResource := o.EDirectResource()
	oldContainer := o.container
	oldContainerFeatureID := o.containerFeatureID

	// resource
	var newResource EResource
	if oldResource != nil {
		if newContainer != nil && !eContainmentFeature(o, newContainer, newContainerFeatureID).IsResolveProxies() {
			list := oldResource.GetContents().(ENotifyingList)
			notifications = list.RemoveWithNotification(o.AsEObject(), notifications)
			o.ESetDirectResource(nil)
			newResource = newContainer.EResource()
		} else {
			oldResource = nil
		}
	} else {
		if oldContainer != nil {
			oldResource = oldContainer.EResource()
		}

		if newContainer != nil {
			newResource = newContainer.EResource()
		}
	}

	if oldResource != nil && oldResource != newResource {
		oldResource.Detached(o)
	}

	if newResource != nil && newResource != oldResource {
		newResource.Attached(o)
	}

	// basic set
	o.container = newContainer
	o.containerFeatureID = newContainerFeatureID

	// notification
	if o.ENotificationRequired() {
		if oldContainer != nil && oldContainerFeatureID >= 0 && oldContainerFeatureID != newContainerFeatureID {
			notification := NewNotificationByFeatureID(o.AsEObject(), SET, oldContainerFeatureID, oldContainer, nil, -1)
			if notifications != nil {
				notifications.Add(notification)
			} else {
				notifications = notification
			}
		}
		if newContainerFeatureID >= 0 {
			var c EObject
			if oldContainerFeatureID == newContainerFeatureID {
				c = oldContainer
			}
			notification := NewNotificationByFeatureID(o.AsEObject(), SET, newContainerFeatureID, c, newContainer, -1)
			if notifications != nil {
				notifications.Add(notification)
			} else {
				notifications = notification
			}
		}
	}
	return notifications
}

// EBasicRemoveFromContainer ...
func (o *BasicEObject) EBasicRemoveFromContainer(notifications ENotificationChain) ENotificationChain {
	if o.containerFeatureID >= 0 {
		return o.EBasicRemoveFromContainerFeature(notifications)
	} else {
		if o.container != nil {
			return o.AsEObjectInternal().EInverseRemove(o.AsEObject(), EOPPOSITE_FEATURE_BASE-o.containerFeatureID, notifications)
		}
	}
	return notifications
}

// EBasicRemoveFromContainerFeature ...
func (o *BasicEObject) EBasicRemoveFromContainerFeature(notifications ENotificationChain) ENotificationChain {
	reference, isReference := o.AsEObject().EClass().GetEStructuralFeature(o.containerFeatureID).(EReference)
	if isReference {
		inverseFeature := reference.GetEOpposite()
		if o.container != nil && inverseFeature != nil {
			return o.AsEObjectInternal().EInverseRemove(o.AsEObject(), inverseFeature.GetFeatureID(), notifications)
		}
	}
	return notifications
}

func (o *BasicEObject) eStructuralFeature(featureName string) EStructuralFeature {
	eFeature := o.AsEObject().EClass().GetEStructuralFeatureFromString(featureName)
	if eFeature == nil {
		panic("The feature " + featureName + " is not a valid feature")
	}
	return eFeature
}

func (o *BasicEObject) EObjectForFragmentSegment(uriSegment string) EObject {

	index := -1
	r, _ := utf8.DecodeLastRuneInString(uriSegment)
	if unicode.IsDigit(r) {
		if index = strings.LastIndex(uriSegment, "."); index != -1 {
			pos, _ := strconv.Atoi(uriSegment[index+1:])
			eFeatureName := uriSegment[1:index]
			eFeature := o.eStructuralFeature(eFeatureName)
			list := o.AsEObject().EGet(eFeature).(EList)
			if pos < list.Size() {
				return list.Get(pos).(EObject)
			}
		}
	}
	if index == -1 {
		eFeature := o.eStructuralFeature(uriSegment)
		return o.AsEObject().EGet(eFeature).(EObject)
	}
	return nil
}

func (o *BasicEObject) EURIFragmentSegment(feature EStructuralFeature, object EObject) string {
	s := "@"
	s += feature.GetName()
	if feature.IsMany() {
		v := o.AsEObject().EGet(feature)
		i := v.(EList).IndexOf(object)
		s += "." + strconv.Itoa(i)
	}
	return s
}
