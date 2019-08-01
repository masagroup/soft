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

func discardEReference() {
	_ = assert.Equal
	_ = mock.Anything
	_ = testing.Coverage

	_ = time.Now()
}

func TestEReferenceContainmentGet(t *testing.T) {
	obj := newEReferenceImpl()
	obj.SetContainment(true)
	assert.Equal(t, true, obj.IsContainment())
}

func TestEReferenceContainmentSet(t *testing.T) {
	mockAdapter := &MockEAdapter{}
	mockAdapter.On("NotifyChanged", mock.Anything).Once()
	obj := newEReferenceImpl()
	obj.EAdapters().Add(mockAdapter)
	obj.SetContainment(true)
	mockAdapter.AssertExpectations(t)
}

func TestEReferenceContainmentEGet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsContainment(), obj.EGetFromID(EREFERENCE__CONTAINMENT, false, false))
	}
	{
		assert.Equal(t, obj.GetEKeys(), obj.EGetFromID(EREFERENCE__EKEYS, false, false))
	}
	{
		assert.Equal(t, obj.GetEOpposite(), obj.EGetFromID(EREFERENCE__EOPPOSITE, false, false))
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsResolveProxies(), obj.EGetFromID(EREFERENCE__RESOLVE_PROXIES, false, false))
	}
}

func TestEReferenceContainmentEIsSet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.eKeys != nil && obj.eKeys.Size() != 0, obj.EIsSetFromID(EREFERENCE__EKEYS))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceContainmentEUnset(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		obj.EUnsetFromID(EREFERENCE__EKEYS)
		obj.GetEKeys().Clear()
		assert.Equal(t, 0, obj.GetEKeys().Size())
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceContainmentESet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		elem := NewEmptyArrayEList()
		obj.ESetFromID(EREFERENCE__EKEYS, elem)
		assert.Equal(t, 1, obj.GetEKeys().Size())
		assert.Equal(t, elem, obj.GetEKeys().Get(0))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceResolveProxiesGet(t *testing.T) {
	obj := newEReferenceImpl()
	obj.SetResolveProxies(true)
	assert.Equal(t, true, obj.IsResolveProxies())
}

func TestEReferenceResolveProxiesSet(t *testing.T) {
	mockAdapter := &MockEAdapter{}
	mockAdapter.On("NotifyChanged", mock.Anything).Once()
	obj := newEReferenceImpl()
	obj.EAdapters().Add(mockAdapter)
	obj.SetResolveProxies(true)
	mockAdapter.AssertExpectations(t)
}

func TestEReferenceResolveProxiesEGet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsContainment(), obj.EGetFromID(EREFERENCE__CONTAINMENT, false, false))
	}
	{
		assert.Equal(t, obj.GetEKeys(), obj.EGetFromID(EREFERENCE__EKEYS, false, false))
	}
	{
		assert.Equal(t, obj.GetEOpposite(), obj.EGetFromID(EREFERENCE__EOPPOSITE, false, false))
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsResolveProxies(), obj.EGetFromID(EREFERENCE__RESOLVE_PROXIES, false, false))
	}
}

func TestEReferenceResolveProxiesEIsSet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.eKeys != nil && obj.eKeys.Size() != 0, obj.EIsSetFromID(EREFERENCE__EKEYS))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceResolveProxiesEUnset(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		obj.EUnsetFromID(EREFERENCE__EKEYS)
		obj.GetEKeys().Clear()
		assert.Equal(t, 0, obj.GetEKeys().Size())
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceResolveProxiesESet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		elem := NewEmptyArrayEList()
		obj.ESetFromID(EREFERENCE__EKEYS, elem)
		assert.Equal(t, 1, obj.GetEKeys().Size())
		assert.Equal(t, elem, obj.GetEKeys().Get(0))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceEOppositeGet(t *testing.T) {
	obj := newEReferenceImpl()
	obj.SetEOpposite(newEReferenceImpl())
	assert.Equal(t, newEReferenceImpl(), obj.GetEOpposite())
}

func TestEReferenceEOppositeSet(t *testing.T) {
	mockAdapter := &MockEAdapter{}
	mockAdapter.On("NotifyChanged", mock.Anything).Once()
	obj := newEReferenceImpl()
	obj.EAdapters().Add(mockAdapter)
	obj.SetEOpposite(newEReferenceImpl())
	mockAdapter.AssertExpectations(t)
}

func TestEReferenceEOppositeEGet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsContainment(), obj.EGetFromID(EREFERENCE__CONTAINMENT, false, false))
	}
	{
		assert.Equal(t, obj.GetEKeys(), obj.EGetFromID(EREFERENCE__EKEYS, false, false))
	}
	{
		assert.Equal(t, obj.GetEOpposite(), obj.EGetFromID(EREFERENCE__EOPPOSITE, false, false))
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsResolveProxies(), obj.EGetFromID(EREFERENCE__RESOLVE_PROXIES, false, false))
	}
}

func TestEReferenceEOppositeEIsSet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.eKeys != nil && obj.eKeys.Size() != 0, obj.EIsSetFromID(EREFERENCE__EKEYS))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceEOppositeEUnset(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		obj.EUnsetFromID(EREFERENCE__EKEYS)
		obj.GetEKeys().Clear()
		assert.Equal(t, 0, obj.GetEKeys().Size())
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceEOppositeESet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		elem := NewEmptyArrayEList()
		obj.ESetFromID(EREFERENCE__EKEYS, elem)
		assert.Equal(t, 1, obj.GetEKeys().Size())
		assert.Equal(t, elem, obj.GetEKeys().Get(0))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceEKeysGetList(t *testing.T) {
	obj := newEReferenceImpl()
	assert.NotNil(t, obj.GetEKeys())
}

func TestEReferenceEKeysEGet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsContainment(), obj.EGetFromID(EREFERENCE__CONTAINMENT, false, false))
	}
	{
		assert.Equal(t, obj.GetEKeys(), obj.EGetFromID(EREFERENCE__EKEYS, false, false))
	}
	{
		assert.Equal(t, obj.GetEOpposite(), obj.EGetFromID(EREFERENCE__EOPPOSITE, false, false))
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.IsResolveProxies(), obj.EGetFromID(EREFERENCE__RESOLVE_PROXIES, false, false))
	}
}

func TestEReferenceEKeysEIsSet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		assert.Equal(t, obj.eKeys != nil && obj.eKeys.Size() != 0, obj.EIsSetFromID(EREFERENCE__EKEYS))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceEKeysEUnset(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		obj.EUnsetFromID(EREFERENCE__EKEYS)
		obj.GetEKeys().Clear()
		assert.Equal(t, 0, obj.GetEKeys().Size())
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}

func TestEReferenceEKeysESet(t *testing.T) {
	obj := newEReferenceImpl()
	{
		_ = obj
	}
	{
		elem := NewEmptyArrayEList()
		obj.ESetFromID(EREFERENCE__EKEYS, elem)
		assert.Equal(t, 1, obj.GetEKeys().Size())
		assert.Equal(t, elem, obj.GetEKeys().Get(0))
	}
	{
		_ = obj
	}
	{
		_ = obj
	}
}
