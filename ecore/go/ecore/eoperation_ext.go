// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// eOperationExt is the extension of the model object 'EOperation'
type eOperationExt struct {
    *eOperationImpl
}

func newEOperationExt() *eOperationExt {
	eOperation := new(eOperationExt)
	eOperation.eOperationImpl = newEOperationImpl()
	eOperation.internal = eOperation
	return eOperation
}
