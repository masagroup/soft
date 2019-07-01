package ecore

// eClassExt is the extension of the model object 'EClass'
type eClassExt struct {
    *eClassImpl
}

func newEClassExt() *eClassExt {
	eClass := new(eClassExt)
	eClass.eClassImpl = newEClassImpl()
	eClass.internal = eClass
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
		allReferences = append( allReferences , superReferences.ToArray() )
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
		allOperations = append( allOperations , superOperations.ToArray() )
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
    eClass.eAllStructuralFeatures = NewEmptyArrayEList();
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
