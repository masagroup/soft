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
	adapter *eSuperAdapter
}

type eSuperAdapter struct {
	*Adapter
	class *eClassExt
	subClasses []*eClassExt
}

func (adapter *eSuperAdapter) NotifyChanged(notification ENotification) {
	eventType := notification.GetEventType()
	eNotifier := notification.GetNotifier().(*eClassExt)
	if eventType != REMOVING_ADAPTER {
		featureID := notification.GetFeatureID()
        if  featureID == ECLASS__ESUPER_TYPES {
			switch eventType {
			case SET:
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
					class := oldValue.(*eClassExt)
					class.adapter.subClasses = append( class.adapter.subClasses , eNotifier )
				}
			case ADD:
				newValue := notification.GetNewValue()
				if newValue != nil {
					class := newValue.(*eClassExt)
					class.adapter.subClasses = append( class.adapter.subClasses , eNotifier )
				}
			case ADD_MANY:
				newValue := notification.GetNewValue()
				if newValue != nil {
					collection := newValue.([]interface{})
					for _, s := range collection { 
						class := s.(*eClassExt)
						class.adapter.subClasses = append( class.adapter.subClasses , eNotifier )
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
        adapter.class.setModified( featureID )
    }
}

func newEClassExt() *eClassExt {
	eClass := new(eClassExt)
	eClass.eClassImpl = newEClassImpl()
	eClass.interfaces = eClass
	eClass.adapter = &eSuperAdapter{ Adapter : NewAdapter(),  class : eClass , subClasses : []*eClassExt{}}
	eClass.EAdapters().Add( eClass.adapter )
	return eClass
}

func (eClass *eClassExt) initEAttributes() {
    eClass.initEAllAttributes()
}

func (eClass *eClassExt) initEReferences() {
    eClass.initEAllReferences();
}

func (eClass *eClassExt) initEContainments() {
    eClass.eContainments = NewEmptyArrayEList();
}

func (eClass *eClassExt) initECrossReferences() {
    eClass.eCrossReferences = NewEmptyArrayEList();
}

func (eClass *eClassExt) initFeaturesSubSet() {

}

func (eClass *eClassExt) initEAllAttributes() {
	if eClass.eAllAttributes != nil {
        return
	}

	attributes := []interface{}{}
	allAttributes := []interface{}{}
	var eIDAttribute EAttribute
	for itClass := eClass.GetESuperTypes().Iterate(); itClass.Next(); {
		superAttributes := itClass.Value().(EClass).GetEAllAttributes()
		for itAttribute := superAttributes.Iterate(); itAttribute.Next(); {
			attribute := itAttribute.Value().(EAttribute)
			allAttributes = append(allAttributes , attribute )
			if ( attribute.IsID() && eClass.eIDAttribute == nil ) {
				eIDAttribute = attribute
			}
		}
	}

	for itFeature := eClass.GetEStructuralFeatures().Iterate(); itFeature.Next(); {
		attribute , isAttribute := itFeature.Value().(EAttribute)
		if isAttribute {
			attributes = append( attributes , attribute )
			allAttributes = append(allAttributes , attribute )
			if ( attribute.IsID() && eClass.eIDAttribute == nil ) {
				eIDAttribute = attribute
			}
		}
	}
	eClass.eIDAttribute = eIDAttribute;
	eClass.eAttributes = NewImmutableEList(attributes)
	eClass.eAllAttributes = NewImmutableEList(allAttributes)
}

func (eClass *eClassExt) initEAllReferences() {
    if eClass.eAllReferences != nil {
		return
	}

	allReferences :=  []interface{}{}
	references := []interface{}{}
	for itClass := eClass.GetESuperTypes().Iterate(); itClass.Next(); {
		superReferences := itClass.Value().(EClass).GetEAllReferences()
		allReferences = append( allReferences , superReferences.ToArray()... )
	}

	for itFeature := eClass.GetEStructuralFeatures().Iterate(); itFeature.Next(); {
		reference , isReference := itFeature.Value().(EReference)
		if isReference {
			references = append( references , reference )
			allReferences = append(allReferences , reference )
		}
	}

    eClass.eReferences = NewImmutableEList(references)
    eClass.eAllReferences = NewImmutableEList(allReferences)
}

func (eClass *eClassExt) initEAllContainments() {
    eClass.eAllContainments = NewEmptyArrayEList();
}

func (eClass *eClassExt) initEAllOperations() {
    if eClass.eAllOperations != nil {
		return
	}
	
	allOperations :=  []interface{}{}
	operations := []interface{}{}
	for itClass := eClass.GetESuperTypes().Iterate(); itClass.Next(); {
		superOperations := itClass.Value().(EClass).GetEAllOperations()
		allOperations = append( allOperations , superOperations.ToArray()... )
	}

	for itFeature := eClass.GetEOperations().Iterate(); itFeature.Next(); {
		operation , isOperation := itFeature.Value().(EOperation)
		if isOperation {
			operations = append( operations , operation )
			allOperations = append(allOperations , operation )
		}
	}

    eClass.eOperations = NewImmutableEList(operations)
    eClass.eAllOperations = NewImmutableEList(allOperations)

}

func (eClass *eClassExt) initEAllStructuralFeatures() {
	if( eClass.eAllStructuralFeatures != nil ) {
		return
	}
    
    eClass.eCrossReferences = nil
    eClass.eContainments = nil
    
	allFeatures := []interface{}{}
	for itClass := eClass.GetESuperTypes().Iterate(); itClass.Next(); {
		superFeatures := itClass.Value().(EClass).GetEAllStructuralFeatures()
		allFeatures = append( allFeatures , superFeatures.ToArray()... )
	}

	featureID := len(allFeatures)
	for itFeature := eClass.GetEStructuralFeatures().Iterate() ; itFeature.Next();  {
		feature := itFeature.Value().(EStructuralFeature)
		feature.SetFeatureID( featureID )
		featureID++
		allFeatures = append( allFeatures , feature )
	}
    eClass.eAllStructuralFeatures = NewImmutableEList(allFeatures);
}

func (eClass *eClassExt) initEAllSuperTypes() {
    eClass.eAllSuperTypes = NewEmptyArrayEList();
}

func (eClass *eClassExt) initEIDAttribute() {
    eClass.initEAllAttributes()
}

func (eClass *eClassExt) setModified( featureID int ) {
	switch( featureID ) {
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
        eClass.eAllStructuralFeatures = nil
        eClass.eAllReferences = nil
        eClass.eAllContainments = nil
	}

}
