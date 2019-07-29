// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// eTypedElementExt is the extension of the model object 'ETypedElement'
type eTypedElementExt struct {
	*eTypedElementImpl
}

func newETypedElementExt() *eTypedElementExt {
	eTypedElement := new(eTypedElementExt)
	eTypedElement.eTypedElementImpl = newETypedElementImpl()
	eTypedElement.interfaces = eTypedElement
	return eTypedElement
}

// IsMany get the value of isMany
func (eTypedElement *eTypedElementExt) IsMany() bool {
    upper := eTypedElement.GetUpperBound()
    return upper > 1 || upper == UNBOUNDED_MULTIPLICITY
}

// IsRequired get the value of isRequired
func (eTypedElement *eTypedElementExt) IsRequired() bool {
    lower := eTypedElement.GetLowerBound()
    return lower >= 1
}