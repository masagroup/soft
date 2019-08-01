// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// DynamicEObjectImpl ...
type DynamicEObjectImpl struct {
	*EObjectImpl
	class EClass
	properties []interface{}
}

// NewDynamicEObjectImpl is the constructor of a DynamicEObjectImpl
func NewDynamicEObjectImpl() *DynamicEObjectImpl {
	o := new(DynamicEObjectImpl)
	o.EObjectImpl = NewEObjectImpl()
	o.SetInterfaces( o )
	return o
}

// EClass ...
func (o *DynamicEObjectImpl) EClass() EClass {
	if ( o.class == nil ) {
		return o.EStaticClass()
	}
	return o.class
}

// SetEClass ...
func (o *DynamicEObjectImpl) SetEClass( class EClass ) {
	o.class = class
}
