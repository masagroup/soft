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
//*****************************************************************************

package ecore

// eAttributeImpl is the implementation of the model object 'EAttribute'
type eAttributeImpl struct {
	*eStructuralFeatureExt
	isID           bool
	eAttributeType EDataType
}

// newEAttributeImpl is the constructor of a eAttributeImpl
func newEAttributeImpl() *eAttributeImpl {
	eAttribute := new(eAttributeImpl)
	eAttribute.eStructuralFeatureExt = newEStructuralFeatureExt()
	eAttribute.SetInterfaces(eAttribute)
	eAttribute.isID = false

	return eAttribute
}

func (eAttribute *eAttributeImpl) EStaticClass() EClass {
	return GetPackage().GetEAttribute()
}

// IsID get the value of isID
func (eAttribute *eAttributeImpl) IsID() bool {
	return eAttribute.isID

}

// SetID set the value of isID
func (eAttribute *eAttributeImpl) SetID(newIsID bool) {
	oldIsID := eAttribute.isID
	eAttribute.isID = newIsID
	if eAttribute.ENotificationRequired() {
		eAttribute.ENotify(NewNotificationByFeatureID(eAttribute.GetEObject(), SET, EATTRIBUTE__ID, oldIsID, newIsID, NO_INDEX))
	}
}

// GetEAttributeType get the value of eAttributeType
func (eAttribute *eAttributeImpl) GetEAttributeType() EDataType {
	panic("GetEAttributeType not implemented")

}

func (eAttribute *eAttributeImpl) basicGetEAttributeType() EDataType {
	panic("GetEAttributeType not implemented")

}

func (eAttribute *eAttributeImpl) EGetFromID(featureID int, resolve, coreType bool) interface{} {
	switch featureID {
	case EATTRIBUTE__EATTRIBUTE_TYPE:
		return eAttribute.GetEAttributeType()
	case EATTRIBUTE__ID:
		return eAttribute.IsID()
	default:
		return eAttribute.eStructuralFeatureExt.EGetFromID(featureID, resolve, coreType)
	}
}

func (eAttribute *eAttributeImpl) ESetFromID(featureID int, newValue interface{}) {
	switch featureID {
	case EATTRIBUTE__ID:
		i := newValue.(bool)
		eAttribute.SetID(i)
	default:
		eAttribute.eStructuralFeatureExt.ESetFromID(featureID, newValue)
	}
}

func (eAttribute *eAttributeImpl) EUnsetFromID(featureID int) {
	switch featureID {
	case EATTRIBUTE__ID:
		eAttribute.SetID(false)
	default:
		eAttribute.eStructuralFeatureExt.EUnsetFromID(featureID)
	}
}

func (eAttribute *eAttributeImpl) EIsSetFromID(featureID int) bool {
	switch featureID {
	case EATTRIBUTE__EATTRIBUTE_TYPE:
		return eAttribute.GetEAttributeType() != nil
	case EATTRIBUTE__ID:
		return eAttribute.isID != false
	default:
		return eAttribute.eStructuralFeatureExt.EIsSetFromID(featureID)
	}
}
