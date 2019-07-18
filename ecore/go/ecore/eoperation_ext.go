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
	eOperation.interfaces = eOperation
	return eOperation
}

func (eOperation *eOperationExt) IsOverrideOf(otherOperation EOperation) bool {
	otherContainingClass := otherOperation.GetEContainingClass()
	if otherContainingClass != nil && otherContainingClass.IsSuperTypeOf(eOperation.GetEContainingClass()) && otherOperation.GetName() == eOperation.GetName() {
		parameters := eOperation.GetEParameters()
		otherParameters := otherOperation.GetEParameters()
		if parameters.Size() == otherParameters.Size() {
			for i := 0; i < parameters.Size(); i++ {
				parameter := parameters.Get(i).(EParameter)
				otherParameter := otherParameters.Get(i).(EParameter)
				if parameter.GetEType() != otherParameter.GetEType() {
					return false
				}
			}
			return true
		}
	}
	return false
}
