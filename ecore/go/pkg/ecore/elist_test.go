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

func TestGet(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Get(0), 3)
	assert.Equal(t, arr.Get(1), 5)
	assert.Equal(t, arr.Get(2), 7)
}

func TestSize(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Size(), 3)
}

func TestAddAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{2})
	arr2.AddAll(arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{2, 3, 5, 7})
}

func TestInsertPrepend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.Equal(t, arr.Insert(0, 2), true)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 3})
}

func TestInsertAppend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2})
}

func TestInsertMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.Insert(2, 3), true)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2, 3, 5, 7})
}

func TestInsertAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{-3, 3, 5, 7, -5, -7})
}

func TestMoveAfter(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(3, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6, 4, 8, 10})
}

func TestMoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(5, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6, 8, 10, 4})
}

func TestMoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(0, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{4, 2, 6, 8, 10})
}

func TestMoveSame(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(1, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 4, 6, 8, 10})
}

func TestRemoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(2)
	assert.Equal(t, arr.ToArray(), []interface{}{4, 6})
}

func TestRemoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(6)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 4})
}

func TestRemoveMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 6})
	assert.Equal(t, arr.Size(), 4)
	arr.Remove(4)
	arr.Remove(6)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6})
}

func TestClear(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.Equal(t, arr.Size(), 2)
	arr.Clear()
	assert.True(t, arr.Empty())
}

func TestEmptyTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.Equal(t, arr.Empty(), false)
}

func TestEmptyFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{})
	assert.Equal(t, arr.Empty(), true)
}

func TestContainsFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.Equal(t, arr.Contains(4), false)
}

func TestContainsTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.Equal(t, arr.Contains(2), true)
}

func TestIterate(t *testing.T) {
	arr := NewArrayEList([]interface{}{0, 2, 4})
	i := 0
	for it := arr.Iterate(); it.Next(); {
		assert.Equal(t, it.Value(), i)
		i += 2
	}
	assert.Equal(t, 6, i)
}

func TestIterateImmutable(t *testing.T) {
	var iDatas []interface{}
	iDatas = append(iDatas, 0, 2, 4, 6)
	arr := NewImmutableEList(iDatas)
	i := 0
	for it := arr.Iterate(); it.Next(); {
		assert.Equal(t, it.Value(), i)
		i += 2
	}
	assert.Equal(t, 8, i)
}

func TestAddAllUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, 7, 5})
	arr2 := NewUniqueArrayEList([]interface{}{2})
	arr2.AddAll(arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{2, 3, 5, 7})
}

func TestAddUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{})
	assert.Equal(t, arr.Add(2), true)
	assert.Equal(t, arr.Add(5), true)
	assert.Equal(t, arr.Add(2), false)
}

func TestInsertUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.Insert(2, 9), true)
	assert.Equal(t, arr.Insert(2, 3), false)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2, 9, 5, 7})
}

func TestInsertAllUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, -5, 7})
	arr2 := NewUniqueArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{-3, 3, 5, 7, -5, -7})
}
