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

// EOperation is the representation of the model object 'EOperation'
type EOperation interface {
	ETypedElement

	IsOverrideOf(EOperation) bool

	GetOperationID() int
	SetOperationID(int)

	GetEContainingClass() EClass

	GetEParameters() EList

	GetEExceptions() EList
	UnsetEExceptions()

	// Start of user code EOperation
	// End of user code
}
