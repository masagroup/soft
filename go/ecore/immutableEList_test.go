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
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestImmutableEListAdd(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.Add(1) })
}

func TestImmutableEListAddAll(t *testing.T) {
	a := NewImmutableEList(nil)
	b := NewImmutableEList([]interface{}{1, 2})
	assert.Panics(t, func() { a.AddAll(b) })
}

func TestImmutableEListInsert(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.Insert(0, 1) })
}

func TestImmutableEListInsertAll(t *testing.T) {
	a := NewImmutableEList(nil)
	b := NewImmutableEList([]interface{}{1, 2})
	assert.Panics(t, func() { a.InsertAll(0, b) })
}

func TestImmutableEListMoveObject(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.MoveObject(0, 1) })
}

func TestImmutableEListMove(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.Move(0, 1) })
}

func TestImmutableEListGet(t *testing.T) {
	a := NewImmutableEList([]interface{}{1, 2})
	assert.Equal(t, 1, a.Get(0))
	assert.Equal(t, 2, a.Get(1))
	assert.Panics(t, func() { a.Get(2) })
}

func TestImmutableEListSet(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.Set(0, 1) })
}

func TestImmutableEListRemoveAt(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.RemoveAt(0) })
}

func TestImmutableEListRemove(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.Remove(0) })
}

func TestImmutableEListSize(t *testing.T) {
	a := NewImmutableEList([]interface{}{1, 2})
	assert.Equal(t, 2, a.Size())
}

func TestImmutableEListEmpty(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.True(t, a.Empty())
}

func TestImmutableEListClear(t *testing.T) {
	a := NewImmutableEList(nil)
	assert.Panics(t, func() { a.Clear() })
}

func TestImmutableEListContains(t *testing.T) {
	a := NewImmutableEList([]interface{}{1, 2})
	assert.True(t, a.Contains(2))
	assert.False(t, a.Contains(3))
	b := NewImmutableEList(nil)
	assert.False(t, b.Contains(3))
}

func TestImmutableEListIterate(t *testing.T) {
	var iDatas []interface{}
	iDatas = append(iDatas, 0, 2, 4, 6)
	arr := NewImmutableEList(iDatas)
	i := 0
	for it := arr.Iterator(); it.HasNext(); {
		assert.Equal(t, it.Next(), i)
		i += 2
	}
	assert.Equal(t, 8, i)
}
