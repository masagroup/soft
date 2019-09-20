// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

import (
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
	"testing"
)

func TestDynamicEObjectConstructor(t *testing.T) {
	o := NewDynamicEObjectImpl()
	assert.NotNil(t, o)
	assert.Equal(t, GetPackage().GetEObject(), o.EClass())
}

func TestDynamicEObject_EClass(t *testing.T) {
	o := NewDynamicEObjectImpl()
	mockClass := &MockEClass{}
	mockAdapters := &MockEList{}
	mockClass.On("GetFeatureCount").Return(0)
	mockClass.On("EAdapters").Return(mockAdapters)
	mockAdapters.On("Add", mock.Anything).Return(true).Once()
	o.SetEClass(mockClass)
	assert.Equal(t, mockClass, o.EClass())
}

func TestDynamicEObject_MockEClass(t *testing.T) {
	o := NewDynamicEObjectImpl()
	c := GetFactory().CreateEClass()
	o.SetEClass(c)
	assert.Equal(t, c, o.EClass())
}

func TestDynamicEObject_GetSet(t *testing.T) {
	o := NewDynamicEObjectImpl()
	c := GetFactory().CreateEClass()
	o.SetEClass(c)
	assert.Equal(t, c, o.EClass())

	f := GetFactory().CreateEAttribute()
	c.GetEStructuralFeatures().Add(f)
	assert.Nil(t, o.EGet(f))

	o.ESet(f, 1)
	assert.Equal(t, 1, o.EGet(f))
}

func TestDynamicEObject_Unset(t *testing.T) {
	o := NewDynamicEObjectImpl()
	c := GetFactory().CreateEClass()
	o.SetEClass(c)
	assert.Equal(t, c, o.EClass())

	f := GetFactory().CreateEAttribute()
	c.GetEStructuralFeatures().Add(f)
	assert.Nil(t, o.EGet(f))

	o.ESet(f, 1)
	assert.Equal(t, 1, o.EGet(f))

	o.EUnset(f)
	assert.Nil(t, o.EGet(f))
}
