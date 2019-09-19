// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// EFactoryExt is the extension of the model object 'EFactory'
type EFactoryExt struct {
	*eFactoryImpl
}

// NewEFactoryExt ...
func NewEFactoryExt() *EFactoryExt {
	eFactory := new(EFactoryExt)
	eFactory.eFactoryImpl = newEFactoryImpl()
	eFactory.SetInterfaces(eFactory)
	return eFactory
}

// Create ...
func (eFactory *EFactoryExt) Create(eClass EClass) EObject {
	if eFactory.GetEPackage() != eClass.GetEPackage() || eClass.IsAbstract() {
		panic("The class '" + eClass.GetName() + "' is not a valid classifier")
	}
	eObject := NewDynamicEObjectImpl()
	eObject.SetEClass(eClass)
	return eObject
}
