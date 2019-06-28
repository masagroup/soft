package ecore

// eClassExt is the extension of the model object 'EClass'
type eClassExt struct {
    *eClassImpl
}

func newEClassExt() *eClassExt {
	eClass := new(eClassExt)
	eClass.eClassImpl = newEClassImpl()
	return eClass
}

func (eClass *eClassExt) initEAttributes() {
    eClass.initEAllAttributes()
}

func (eClass *eClassExt) initEReferences() {
    eClass.eReferences = NewEmptyArrayEList();
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
        return;
	}

	attributes := []interface{}{}
	allAttributes := []interface{}{}
	var eIDAttribute EAttribute
	for itClass := eClass.GetESuperTypes().Iterate(); itClass.Next(); {
		superAttributes := itClass.Value().(EClass).GetEAllAttributes();
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
	eClass.eAttributes = NewImmutableEList(attributes);
	eClass.eAllAttributes = NewImmutableEList(allAttributes);
}

func (eClass *eClassExt) initEAllReferences() {
    eClass.eAllReferences = NewEmptyArrayEList();
}

func (eClass *eClassExt) initEAllContainments() {
    eClass.eAllContainments = NewEmptyArrayEList();
}

func (eClass *eClassExt) initEAllOperations() {
    eClass.eAllOperations = NewEmptyArrayEList();
}

func (eClass *eClassExt) initEAllStructuralFeatures() {
    eClass.eAllStructuralFeatures = NewEmptyArrayEList();
}

func (eClass *eClassExt) initEAllSuperTypes() {
    eClass.eAllSuperTypes = NewEmptyArrayEList();
}

func (eClass *eClassExt) initEIDAttribute() {
    panic("initEIDAttribute not implemented")
}