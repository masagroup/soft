package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestArrayEListGet(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Get(0), 3)
	assert.Equal(t, arr.Get(1), 5)
	assert.Equal(t, arr.Get(2), 7)
}

func TestArrayEListSize(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Size(), 3)
}

func TestArrayEListAddAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{2})
	arr2.AddAll(arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{2, 3, 5, 7})
}

func TestArrayEListInsertPrepend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.Equal(t, arr.Insert(0, 2), true)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 3})
	assert.Panics(t, func(){ arr.Insert(-1,1) } )
}

func TestArrayEListInsertAppend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2})
	assert.Panics(t, func(){ arr.Insert(3,1) } )
}

func TestArrayEListInsertMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.Insert(2, 3), true)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2, 3, 5, 7})
}

func TestArrayEListInsertAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{-3, 3, 5, 7, -5, -7})
	assert.Panics(t, func(){ arr.InsertAll(-1,arr) } )
	assert.Panics(t, func(){ arr.InsertAll(6,arr) } )
}

func TestArrayEListMoveAfter(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(3, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6, 4, 8, 10})
}

func TestArrayEListMoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(5, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6, 8, 10, 4})
}

func TestArrayEListMoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(0, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{4, 2, 6, 8, 10})
}

func TestArrayEListMoveSame(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(1, 4)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 4, 6, 8, 10})
}

func TestArrayEListMoveInvalid(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	assert.Panics(t, func(){ arr.MoveObject(1,3) } )
}


func TestArrayEListRemoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(2)
	assert.Equal(t, arr.ToArray(), []interface{}{4, 6})
}

func TestArrayEListRemoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(6)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 4})
}

func TestArrayEListRemoveMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 6})
	assert.Equal(t, arr.Size(), 4)
	arr.Remove(4)
	arr.Remove(6)
	assert.Equal(t, arr.ToArray(), []interface{}{2, 6})
}

func TestArrayEListClear(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.Equal(t, arr.Size(), 2)
	arr.Clear()
	assert.True(t, arr.Empty())
}

func TestArrayEListEmptyTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.Equal(t, arr.Empty(), false)
}

func TestArrayEListEmptyFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{})
	assert.Equal(t, arr.Empty(), true)
}

func TestArrayEListContainsFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.Equal(t, arr.Contains(4), false)
}

func TestArrayEListContainsTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.Equal(t, arr.Contains(2), true)
}

func TestArrayEListIterate(t *testing.T) {
	arr := NewArrayEList([]interface{}{0, 2, 4})
	i := 0
	for it := arr.Iterator(); it.HasNext(); {
		assert.Equal(t, it.Next(), i)
		i += 2
	}
	assert.Equal(t, 6, i)
}

func TestArrayEListAddAllUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, 7, 5})
	arr2 := NewUniqueArrayEList([]interface{}{2})
	arr2.AddAll(arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{2, 3, 5, 7})
}

func TestArrayEListAddUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{})
	assert.Equal(t, arr.Add(2), true)
	assert.Equal(t, arr.Add(5), true)
	assert.Equal(t, arr.Add(2), false)
}

func TestArrayEListInsertUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.Insert(2, 9), true)
	assert.Equal(t, arr.Insert(2, 3), false)
	assert.Equal(t, arr.ToArray(), []interface{}{3, 2, 9, 5, 7})
}

func TestArrayEListInsertAllUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, -5, 7})
	arr2 := NewUniqueArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{-3, 3, 5, 7, -5, -7})
}

