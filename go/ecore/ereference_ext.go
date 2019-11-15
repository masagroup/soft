// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// eReferenceExt is the extension of the model object 'EReference'
type eReferenceExt struct {
	*eReferenceImpl
	eReferenceType EClass
}

func newEReferenceExt() *eReferenceExt {
	eReference := new(eReferenceExt)
	eReference.eReferenceImpl = newEReferenceImpl()
	eReference.interfaces = eReference
	return eReference
}

func (eReference *eReferenceExt) IsContainer() bool {
	opposite := eReference.interfaces.(EReference).GetEOpposite()
	return opposite != nil && opposite.IsContainment()
}

func (eReference *eReferenceExt) GetEReferenceType() EClass {
	if eReference.eReferenceType == nil {
		eType := eReference.GetEType()
		eReferenceType, _ := eType.(EClass)
		if eReferenceType != nil {
			eReference.eReferenceType = eReferenceType
		}
	}
	return eReference.eReferenceType
}

func (eReference *eReferenceExt) basicGetEReferenceType() EClass {
	if eReference.eReferenceType == nil {
		eType := eReference.basicGetEType()
		eReferenceType, _ := eType.(EClass)
		if eReferenceType != nil {
			eReference.eReferenceType = eReferenceType
		}
	}
	return eReference.eReferenceType
}
