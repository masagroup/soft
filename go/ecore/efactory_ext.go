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

func NewEFactoryExt() *EFactoryExt {
	eFactory := new(EFactoryExt)
	eFactory.eFactoryImpl = newEFactoryImpl()
	eFactory.SetInterfaces(eFactory)
	return eFactory
}
