// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

// *****************************************************************************
//
// Warning: This file was generated by soft.generator.go Generator
//
// *****************************************************************************

package ecore

// eNamedElementImpl is the implementation of the model object 'ENamedElement'
type eNamedElementImpl struct {
	*eModelElementExt
	name string
}

// newENamedElementImpl is the constructor of a eNamedElementImpl
func newENamedElementImpl() *eNamedElementImpl {
	eNamedElement := new(eNamedElementImpl)
	eNamedElement.eModelElementExt = newEModelElementExt()
	eNamedElement.SetInterfaces(eNamedElement)
	eNamedElement.name = ""

	return eNamedElement
}

func (eNamedElement *eNamedElementImpl) EStaticClass() EClass {
	return GetPackage().GetENamedElement()
}

// GetName get the value of name
func (eNamedElement *eNamedElementImpl) GetName() string {
	return eNamedElement.name

}

// SetName set the value of name
func (eNamedElement *eNamedElementImpl) SetName(newName string) {
	oldName := eNamedElement.name
	eNamedElement.name = newName
	if eNamedElement.ENotificationRequired() {
		eNamedElement.ENotify(NewNotificationByFeatureID(eNamedElement.GetEObject(), SET, ENAMED_ELEMENT__NAME, oldName, newName, NO_INDEX))
	}
}

func (eNamedElement *eNamedElementImpl) EGetFromID(featureID int, resolve, coreType bool) interface{} {
	switch featureID {
	case ENAMED_ELEMENT__NAME:
		return eNamedElement.GetName()
	default:
		return eNamedElement.eModelElementExt.EGetFromID(featureID, resolve, coreType)
	}
}

func (eNamedElement *eNamedElementImpl) ESetFromID(featureID int, newValue interface{}) {
	switch featureID {
	case ENAMED_ELEMENT__NAME:
		n := newValue.(string)
		eNamedElement.SetName(n)
	default:
		eNamedElement.eModelElementExt.ESetFromID(featureID, newValue)
	}
}

func (eNamedElement *eNamedElementImpl) EUnsetFromID(featureID int) {
	switch featureID {
	case ENAMED_ELEMENT__NAME:
		eNamedElement.SetName("")
	default:
		eNamedElement.eModelElementExt.EUnsetFromID(featureID)
	}
}

func (eNamedElement *eNamedElementImpl) EIsSetFromID(featureID int) bool {
	switch featureID {
	case ENAMED_ELEMENT__NAME:
		return eNamedElement.name != ""
	default:
		return eNamedElement.eModelElementExt.EIsSetFromID(featureID)
	}
}
