// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// EPackageExt is the extension of the model object 'EFactory'
type EPackageExt struct {
	*ePackageImpl
}

func NewEPackageExt() *EPackageExt {
	ePackage := new(EPackageExt)
	ePackage.ePackageImpl = newEPackageImpl()
	ePackage.SetInterfaces(ePackage)
	return ePackage
}
