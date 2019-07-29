// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// eClassExt is the extension of the model object 'EClass'
type eClassExt struct {
	*eClassImpl
	adapter                *eSuperAdapter
	nameToFeatureMap       map[string]EStructuralFeature
	operationToOverrideMap map[EOperation]EOperation
}

type eSuperAdapter struct {
	*Adapter
	class      *eClassExt
	subClasses []*eClassExt
}

func (adapter *eSuperAdapter) NotifyChanged(notification ENotification) {
	eventType := notification.GetEventType()
	eNotifier := notification.GetNotifier().(*eClassExt)
	if eventType != REMOVING_ADAPTER {
		featureID := notification.GetFeatureID()
		if featureID == ECLASS__ESUPER_TYPES {
			switch eventType {
			case SET:
				fallthrough
			case RESOLVE:
				oldValue := notification.GetOldValue()
				if oldValue != nil {
					class := oldValue.(*eClassExt)
					for i, s := range class.adapter.subClasses {
						if s == eNotifier {
							class.adapter.subClasses = append(class.adapter.subClasses[:i], class.adapter.subClasses[i+1:]...)
							break
						}
					}
				}
				newValue := notification.GetNewValue()
				if newValue != nil {
					class := newValue.(*eClassExt)
					class.adapter.subClasses = append(class.adapter.subClasses, eNotifier)
				}
			case ADD:
				newValue := notification.GetNewValue()
				if newValue != nil {
					class := newValue.(*eClassExt)
					class.adapter.subClasses = append(class.adapter.subClasses, eNotifier)
				}
			case ADD_MANY:
				newValue := notification.GetNewValue()
				if newValue != nil {
					collection := newValue.([]interface{})
					for _, s := range collection {
						class := s.(*eClassExt)
						class.adapter.subClasses = append(class.adapter.subClasses, eNotifier)
					}
				}
			case REMOVE:
				oldValue := notification.GetOldValue()
				if oldValue != nil {
					class := oldValue.(*eClassExt)
					for i, s := range class.adapter.subClasses {
						if s == eNotifier {
							class.adapter.subClasses = append(class.adapter.subClasses[:i], class.adapter.subClasses[i+1:]...)
							break
						}
					}
				}
			case REMOVE_MANY:
				oldValue := notification.GetOldValue()
				if oldValue != nil {
					collection := oldValue.([]interface{})
					for _, s := range collection {
						class := s.(*eClassExt)
						for i, s := range class.adapter.subClasses {
							if s == eNotifier {
								class.adapter.subClasses = append(class.adapter.subClasses[:i], class.adapter.subClasses[i+1:]...)
								break
							}
						}
					}
				}
			}
		}
		adapter.class.setModified(featureID)
	}
}

func newEClassExt() *eClassExt {
	eClass := new(eClassExt)
	eClass.eClassImpl = newEClassImpl()
	eClass.interfaces = eClass
	eClass.adapter = &eSuperAdapter{Adapter: NewAdapter(), class: eClass, subClasses: []*eClassExt{}}
	eClass.EAdapters().Add(eClass.adapter)
	return eClass
}

func (eClass *eClassExt) IsSuperTypeOf(someClass EClass) bool {
	return someClass == eClass || (someClass != nil && someClass.GetEAllSuperTypes().Contains(eClass))
}

func (eClass *eClassExt) GetFeatureCount() int {
	return eClass.GetEAllStructuralFeatures().Size()
}

func (eClass *eClassExt) GetEStructuralFeature(featureID int) EStructuralFeature {
	features := eClass.GetEAllStructuralFeatures()
	if featureID >= 0 && featureID < features.Size() {
		return features.Get(featureID).(EStructuralFeature)
	}
	return nil
}

func (eClass *eClassExt) GetEStructuralFeatureFromString(featureName string) EStructuralFeature {
	eClass.initNameToFeatureMap()
	return eClass.nameToFeatureMap[featureName]
}

func (eClass *eClassExt) initNameToFeatureMap() {
	eClass.initEAllStructuralFeatures()

	if eClass.nameToFeatureMap != nil {
		return
	}
	eClass.nameToFeatureMap = make(map[string]EStructuralFeature)
	for itFeature := eClass.eAllStructuralFeatures.Iterator(); itFeature.HasNext(); {
		feature := itFeature.Next().(EStructuralFeature)
		eClass.nameToFeatureMap[feature.GetName()] = feature
	}
}

func (eClass *eClassExt) GetFeatureID(feature EStructuralFeature) int {
	features := eClass.GetEAllStructuralFeatures()
	featureID := feature.GetFeatureID()
	if featureID != -1 {
		for ; featureID < features.Size(); featureID++ {
			if features.Get(featureID) == feature {
				return featureID
			}
		}
	}
	return -1
}

func (eClass *eClassExt) GetOperationCount() int {
	return eClass.GetEAllOperations().Size()
}

func (eClass *eClassExt) GetEOperation(operationID int) EOperation {
	operations := eClass.GetEAllOperations()
	if operationID >= 0 && operationID < operations.Size() {
		return operations.Get(operationID).(EOperation)
	}
	return nil
}

func (eClass *eClassExt) GetOperationID(operation EOperation) int {
	operations := eClass.GetEAllOperations()
	operationID := operation.GetOperationID()
	if operationID != -1 {
		for ; operationID < operations.Size(); operationID++ {
			if operations.Get(operationID) == operation {
				return operationID
			}
		}
	}
	return -1
}

func (eClass *eClassExt) GetOverride(operation EOperation) EOperation {
	eClass.initOperationToOverrideMap()
	return eClass.operationToOverrideMap[operation]
}

func (eClass *eClassExt) initOperationToOverrideMap() {
	eClass.initEAllOperations()

	if eClass.operationToOverrideMap != nil {
		return
	}

	eClass.operationToOverrideMap = make(map[EOperation]EOperation)
	size := eClass.eAllOperations.Size()
	for i := 0; i < size; i++ {
		for j := size - 1; j > i; j-- {
			oi := eClass.eAllOperations.Get(i).(EOperation)
			oj := eClass.eAllOperations.Get(j).(EOperation)
			if oj.IsOverrideOf(oi) {
				eClass.operationToOverrideMap[oi] = oj
			}
		}
	}
}

func (eClass *eClassExt) initEAttributes() {
	eClass.initEAllAttributes()
}

func (eClass *eClassExt) initEReferences() {
	eClass.initEAllReferences()
}

func (eClass *eClassExt) initEContainments() {
	eClass.initFeaturesSubSet()
}

func (eClass *eClassExt) initECrossReferences() {
	eClass.initFeaturesSubSet()
}

func (eClass *eClassExt) initFeaturesSubSet() {
	eClass.initEAllStructuralFeatures()

	if eClass.eContainments != nil {
		return
	}

	containments := []interface{}{}
	crossReferences := []interface{}{}
	for itFeature := eClass.GetEStructuralFeatures().Iterator(); itFeature.HasNext(); {
		ref, isRef := itFeature.Next().(EReference)
		if isRef {
			if ref.IsContainment() {
				if !ref.IsDerived() {
					containments = append(containments, ref)
				}
			} else if !ref.IsContainer() {
				if !ref.IsDerived() {
					crossReferences = append(crossReferences, ref)
				}
			}
		}

	}
	eClass.eContainments = NewImmutableEList(containments)
	eClass.eCrossReferences = NewImmutableEList(crossReferences)
}

func (eClass *eClassExt) initEAllAttributes() {
	if eClass.eAllAttributes != nil {
		return
	}

	attributes := []interface{}{}
	allAttributes := []interface{}{}
	var eIDAttribute EAttribute
	for itClass := eClass.GetESuperTypes().Iterator(); itClass.HasNext(); {
		superAttributes := itClass.Next().(EClass).GetEAllAttributes()
		for itAttribute := superAttributes.Iterator(); itAttribute.HasNext(); {
			attribute := itAttribute.Next().(EAttribute)
			allAttributes = append(allAttributes, attribute)
			if attribute.IsID() && eClass.eIDAttribute == nil {
				eIDAttribute = attribute
			}
		}
	}

	for itFeature := eClass.GetEStructuralFeatures().Iterator(); itFeature.HasNext(); {
		attribute, isAttribute := itFeature.Next().(EAttribute)
		if isAttribute {
			attributes = append(attributes, attribute)
			allAttributes = append(allAttributes, attribute)
			if attribute.IsID() && eClass.eIDAttribute == nil {
				eIDAttribute = attribute
			}
		}
	}
	eClass.eIDAttribute = eIDAttribute
	eClass.eAttributes = NewImmutableEList(attributes)
	eClass.eAllAttributes = NewImmutableEList(allAttributes)
}

func (eClass *eClassExt) initEAllReferences() {
	if eClass.eAllReferences != nil {
		return
	}

	allReferences := []interface{}{}
	references := []interface{}{}
	for itClass := eClass.GetESuperTypes().Iterator(); itClass.HasNext(); {
		superReferences := itClass.Next().(EClass).GetEAllReferences()
		allReferences = append(allReferences, superReferences.ToArray()...)
	}

	for itFeature := eClass.GetEStructuralFeatures().Iterator(); itFeature.HasNext(); {
		reference, isReference := itFeature.Next().(EReference)
		if isReference {
			references = append(references, reference)
			allReferences = append(allReferences, reference)
		}
	}

	eClass.eReferences = NewImmutableEList(references)
	eClass.eAllReferences = NewImmutableEList(allReferences)
}

func (eClass *eClassExt) initEAllContainments() {
	if eClass.eAllContainments != nil {
		return
	}
	allContainments := []interface{}{}
	for itReference := eClass.GetEAllReferences().Iterator(); itReference.HasNext(); {
		reference := itReference.Next().(EReference)
		if reference.IsContainment() {
			allContainments = append(allContainments, reference)
		}
	}
	eClass.eAllContainments = NewImmutableEList(allContainments)
}

func (eClass *eClassExt) initEAllOperations() {
	if eClass.eAllOperations != nil {
		return
	}

	eClass.operationToOverrideMap = nil

	allOperations := []interface{}{}
	for itClass := eClass.GetESuperTypes().Iterator(); itClass.HasNext(); {
		superOperations := itClass.Next().(EClass).GetEAllOperations()
		allOperations = append(allOperations, superOperations.ToArray()...)
	}

	operationID := len(allOperations)
	for itFeature := eClass.GetEOperations().Iterator(); itFeature.HasNext(); {
		operation, isOperation := itFeature.Next().(EOperation)
		if isOperation {
			operation.SetOperationID(operationID)
			operationID++
			allOperations = append(allOperations, operation)
		}
	}
	eClass.eAllOperations = NewImmutableEList(allOperations)
}

func (eClass *eClassExt) initEAllStructuralFeatures() {
	if eClass.eAllStructuralFeatures != nil {
		return
	}

	eClass.eCrossReferences = nil
	eClass.eContainments = nil
	eClass.nameToFeatureMap = nil

	allFeatures := []interface{}{}
	for itClass := eClass.GetESuperTypes().Iterator(); itClass.HasNext(); {
		superFeatures := itClass.Next().(EClass).GetEAllStructuralFeatures()
		allFeatures = append(allFeatures, superFeatures.ToArray()...)
	}

	featureID := len(allFeatures)
	for itFeature := eClass.GetEStructuralFeatures().Iterator(); itFeature.HasNext(); {
		feature := itFeature.Next().(EStructuralFeature)
		feature.SetFeatureID(featureID)
		featureID++
		allFeatures = append(allFeatures, feature)
	}
	eClass.eAllStructuralFeatures = NewImmutableEList(allFeatures)
}

func (eClass *eClassExt) initEAllSuperTypes() {
	if eClass.eAllSuperTypes != nil {
		return
	}
	allSuperTypes := []interface{}{}
	for itClass := eClass.GetESuperTypes().Iterator(); itClass.HasNext(); {
		superClass := itClass.Next().(EClass)
		superTypes := superClass.GetEAllSuperTypes()
		allSuperTypes = append(allSuperTypes, superTypes.ToArray()...)
		allSuperTypes = append(allSuperTypes, superClass)
	}
	eClass.eAllSuperTypes = NewImmutableEList(allSuperTypes)
}

func (eClass *eClassExt) initEIDAttribute() {
	eClass.initEAllAttributes()
}

func (eClass *eClassExt) setModified(featureID int) {
	switch featureID {
	case ECLASS__ESTRUCTURAL_FEATURES:
		eClass.eAllAttributes = nil
		eClass.eAllStructuralFeatures = nil
		eClass.eAllReferences = nil
		eClass.eAllContainments = nil
	case ECLASS__EATTRIBUTES:
		eClass.eAllAttributes = nil
		eClass.eAllStructuralFeatures = nil
		eClass.eAllContainments = nil
	case ECLASS__EREFERENCES:
		eClass.eAllReferences = nil
		eClass.eAllStructuralFeatures = nil
		eClass.eAllContainments = nil
	case ECLASS__EOPERATIONS:
		eClass.eAllOperations = nil
		eClass.eAllContainments = nil
	case ECLASS__ESUPER_TYPES:
		eClass.eAllSuperTypes = nil
		eClass.eAllAttributes = nil
		eClass.eAllOperations = nil
		eClass.eAllStructuralFeatures = nil
		eClass.eAllReferences = nil
		eClass.eAllContainments = nil
	}
	for _, s := range eClass.adapter.subClasses {
		s.setModified(featureID)
	}
}
