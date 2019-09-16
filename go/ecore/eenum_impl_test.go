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

import (
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
)

func discardEEnum() {
	_ = assert.Equal
	_ = mock.Anything
	_ = testing.Coverage

	_ = time.Now()
}

func TestEEnumELiteralsGetList(t *testing.T) {
	obj := newEEnumImpl()
	assert.NotNil(t, obj.GetELiterals())
}

func TestEEnumELiteralsEGet(t *testing.T) {
	obj := newEEnumImpl()
	{
		assert.Equal(t, obj.GetELiterals(), obj.EGetFromID(EENUM__ELITERALS, false, false))
	}
}

func TestEEnumELiteralsEInvoke(t *testing.T) {
	obj := newEEnumImpl()
	{
		assert.Panics(t, func() { obj.EInvokeFromID(EENUM__GET_EENUM_LITERAL_ESTRING, nil) })
	}
	{
		assert.Panics(t, func() { obj.EInvokeFromID(EENUM__GET_EENUM_LITERAL_EINT, nil) })
	}
	{
		assert.Panics(t, func() { obj.EInvokeFromID(EENUM__GET_EENUM_LITERAL_BY_LITERAL_ESTRING, nil) })
	}
}

func TestEEnumELiteralsEIsSet(t *testing.T) {
	obj := newEEnumImpl()
	{
		assert.Equal(t, obj.eLiterals != nil && obj.eLiterals.Size() != 0, obj.EIsSetFromID(EENUM__ELITERALS))
	}
}

func TestEEnumELiteralsEUnset(t *testing.T) {
	obj := newEEnumImpl()
	{
		obj.EUnsetFromID(EENUM__ELITERALS)
		obj.GetELiterals().Clear()
		assert.Equal(t, 0, obj.GetELiterals().Size())
	}
}

func TestEEnumELiteralsESet(t *testing.T) {
	obj := newEEnumImpl()
	{
		elem := NewEmptyArrayEList()
		obj.ESetFromID(EENUM__ELITERALS, elem)
		assert.Equal(t, 1, obj.GetELiterals().Size())
		assert.Equal(t, elem, obj.GetELiterals().Get(0))
	}
}

func TestEEnumELiteralsEInverseAdd(t *testing.T) {
	{
		obj := newEEnumImpl()
		obj.EBasicInverseAdd(&MockEObject{}, EENUM__ELITERALS, &MockENotificationChain{})
		assert.Equal(t, 1, obj.GetELiterals().Size())
	}
}

func TestEEnumELiteralsEInverseRemove(t *testing.T) {
	{
		obj := newEEnumImpl()
		mock := &MockEObject{}
		obj.GetELiterals().Add(mock)
		obj.EBasicInverseRemove(mock, EENUM__ELITERALS, &MockENotificationChain{})
		assert.Equal(t, 0, obj.GetELiterals().Size())
	}
}

func TestEEnumGetEEnumLiteralByNameOperation(t *testing.T) {
	obj := newEEnumImpl()
	assert.Panics(t, func() { obj.GetEEnumLiteralByName("Test String") })
}
func TestEEnumGetEEnumLiteralByValueOperation(t *testing.T) {
	obj := newEEnumImpl()
	assert.Panics(t, func() { obj.GetEEnumLiteralByValue(45) })
}
func TestEEnumGetEEnumLiteralByLiteralOperation(t *testing.T) {
	obj := newEEnumImpl()
	assert.Panics(t, func() { obj.GetEEnumLiteralByLiteral("Test String") })
}
