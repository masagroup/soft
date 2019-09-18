// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// DynamicEObjectImpl ...
type DynamicEObjectImpl struct {
	*EObjectImpl
	class      EClass
	properties []interface{}
}

// NewDynamicEObjectImpl is the constructor of a DynamicEObjectImpl
func NewDynamicEObjectImpl() *DynamicEObjectImpl {
	o := new(DynamicEObjectImpl)
	o.EObjectImpl = NewEObjectImpl()
	o.SetInterfaces(o)
	return o
}

// EClass ...
func (o *DynamicEObjectImpl) EClass() EClass {
	if o.class == nil {
		return o.EStaticClass()
	}
	return o.class
}

// SetEClass ...
func (o *DynamicEObjectImpl) SetEClass(class EClass) {
	o.class = class
}

func (o *DynamicEObjectImpl) EGetFromID(featureID int, resolve bool, core bool) interface{} {
	dynamicFeatureID := featureID - o.eStaticFeatureCount()
	if dynamicFeatureID >= 0 {
		feature := o.eDynamicFeature(featureID)

		// retrieve value or compute it if empty
		result := o.properties[dynamicFeatureID]
		if result == nil {
			if feature.IsMany() {
				result = o.createList(feature)
			}
			o.properties[dynamicFeatureID] = result
		}
		return result
	}
	return o.EObjectImpl.EGetFromID(featureID, resolve, core)
}

func (o *DynamicEObjectImpl) eStaticFeatureCount() int {
	return o.EStaticClass().GetFeatureCount()
}

func (o *DynamicEObjectImpl) eStaticOperationCount() int {
	return o.EStaticClass().GetOperationCount()
}

func (o *DynamicEObjectImpl) eDynamicFeatureID(feature EStructuralFeature) int {
	return o.EClass().GetFeatureID(feature) - o.eStaticFeatureCount()
}

func (o *DynamicEObjectImpl) eDynamicFeature(dynamicFeatureID int) EStructuralFeature {
	return o.EClass().GetEStructuralFeature(dynamicFeatureID + o.eStaticFeatureCount())
}

func (o *DynamicEObjectImpl) isBidirectional(feature EStructuralFeature) bool {
	ref, isRef := feature.(EReference)
	if isRef {
		return ref.GetEOpposite() != nil
	}
	return false
}

func (o *DynamicEObjectImpl) isContainer(feature EStructuralFeature) bool {
	ref, isRef := feature.(EReference)
	if isRef {
		opposite := ref.GetEOpposite()
		if opposite != nil {
			return opposite.IsContainment()
		}
	}
	return false
}

func (o *DynamicEObjectImpl) isContains(feature EStructuralFeature) bool {
	ref, isRef := feature.(EReference)
	if isRef {
		return ref.IsContainment()
	}
	return false
}

func (o *DynamicEObjectImpl) isBackReference(feature EStructuralFeature) bool {
	ref, isRef := feature.(EReference)
	if isRef {
		return ref.IsContainer()
	}
	return false
}

func (o *DynamicEObjectImpl) isProxy(feature EStructuralFeature) bool {
	if o.isContainer(feature) || o.isContains(feature) {
		return false
	}

	ref, isRef := feature.(EReference)
	if isRef {
		return ref.IsResolveProxies()
	}
	return false
}

func (o *DynamicEObjectImpl) createList(feature EStructuralFeature) EList {
	if attribute, isAttribute := feature.(EAttribute); isAttribute {
		if attribute.IsUnique() {
			return NewUniqueArrayEList(nil)
		} else {
			return NewArrayEList(nil)
		}
	} else if ref, isRef := feature.(EReference); isRef {
		inverse := false
		opposite := false
		reverseID := -1
		reverseFeature := ref.GetEOpposite()
		if reverseFeature != nil {
			reverseID = reverseFeature.GetFeatureID()
			inverse = true
			opposite = true
		} else if ref.IsContainment() {
			inverse = true
			opposite = false
		}
		return NewEObjectEList(o.GetEObjectInternal(), ref.GetFeatureID(), reverseID, ref.IsContainment(), inverse, opposite, ref.EIsProxy(), ref.IsUnsettable())
	}
	return nil
}
