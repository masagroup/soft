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

func TestListGet(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Get(0), 3)
	assert.Equal(t, arr.Get(1), 5)
	assert.Equal(t, arr.Get(2), 7)
}

func TestListSize(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Size(), 3)
}

func TestListAddAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{2})
	arr2.AddAll(arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{2, 3, 5, 7})
}

func TestListInsertPrepend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.Equal(t, arr.Insert(0, 2), true)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 3})
	assert.Panics(t, func(){ arr.Insert(-1,1) } )
}

func TestListInsertAppend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2})
	assert.Panics(t, func(){ arr.Insert(3,1) } )
}

func TestListInsertMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.Insert(2, 3), true)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2, 3, 5, 7})
}

func TestListInsertAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{-3, 3, 5, 7, -5, -7})
	assert.Panics(t, func(){ arr.InsertAll(-1,arr) } )
	assert.Panics(t, func(){ arr.InsertAll(6,arr) } )
}

func TestListMoveAfter(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(3, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6, 4, 8, 10})
}

func TestListMoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(5, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6, 8, 10, 4})
}

func TestListMoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(0, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{4, 2, 6, 8, 10})
}

func TestListMoveSame(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(1, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 4, 6, 8, 10})
}

func TestListMoveInvalid(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	assert.Panics(t, func(){ arr.MoveObject(1,3) } )
}


func TestListRemoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(2)
	assert.Equal(t, arr.ToArray(), []interface{}{4, 6})
}

func TestListRemoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(6)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 4})
}

func TestListRemoveMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 6})
	assert.Equal(t, arr.Size(), 4)
	arr.Remove(4)
	arr.Remove(6)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6})
}

func TestListClear(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.Equal(t, arr.Size(), 2)
	arr.Clear()
	assert.True(t, arr.Empty())
}

func TestListEmptyTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.Equal(t, arr.Empty(), false)
}

func TestListEmptyFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{})
	assert.Equal(t, arr.Empty(), true)
}

func TestListContainsFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.Equal(t, arr.Contains(4), false)
}

func TestListContainsTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.Equal(t, arr.Contains(2), true)
}

func TestListIterate(t *testing.T) {
	arr := NewArrayEList([]interface{}{0, 2, 4})
	i := 0
	for it := arr.Iterator(); it.HasNext(); {
		assert.Equal(t, it.Next(), i)
		i += 2
	}
	assert.Equal(t, 6, i)
}

func TestListIterateImmutable(t *testing.T) {
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

func TestListAddAllUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, 7, 5})
	arr2 := NewUniqueArrayEList([]interface{}{2})
	arr2.AddAll(arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{2, 3, 5, 7})
}

func TestListAddUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{})
	assert.Equal(t, arr.Add(2), true)
	assert.Equal(t, arr.Add(5), true)
	assert.Equal(t, arr.Add(2), false)
}

func TestListInsertUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.Insert(2, 9), true)
	assert.Equal(t, arr.Insert(2, 3), false)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2, 9, 5, 7})
}

func TestListInsertAllUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, -5, 7})
	arr2 := NewUniqueArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{-3, 3, 5, 7, -5, -7})
}
