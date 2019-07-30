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
	assert.Panics(t, func() { arr.Get(3) })
}

func TestArrayEListSet(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr.Set(0, 4)
	arr.Set(1, 6)
	arr.Set(2, 8)
	assert.Equal(t, []interface{}{4, 6, 8}, arr.ToArray())
	assert.Panics(t, func() { arr.Set(3, 1) })
}

func TestArrayEListSize(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Size(), 3)
}

func TestArrayEListAddAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{2})
	assert.True(t, arr2.AddAll(arr))
	assert.Equal(t, []interface{}{2, 3, 5, 7}, arr2.ToArray())
}

func TestArrayEListInsertPrepend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.True(t, arr.Insert(0, 2))
	assert.Equal(t, []interface{}{2, 3}, arr.ToArray())
	assert.Panics(t, func() { arr.Insert(-1, 1) })
}

func TestArrayEListInsertAppend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	assert.True(t, arr.Insert(1, 2))
	assert.Equal(t, []interface{}{3, 2}, arr.ToArray())
	assert.Panics(t, func() { arr.Insert(3, 1) })
}

func TestArrayEListInsertMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	assert.True(t, arr.Insert(1, 2))
	assert.True(t, arr.Insert(2, 3))
	assert.Equal(t, []interface{}{3, 2, 3, 5, 7}, arr.ToArray())
}

func TestArrayEListInsertAll(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	arr2 := NewArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, []interface{}{-3, 3, 5, 7, -5, -7}, arr2.ToArray())
	assert.Panics(t, func() { arr.InsertAll(-1, arr) })
	assert.Panics(t, func() { arr.InsertAll(6, arr) })
}

func TestArrayEListMoveObjectAfter(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(3, 4)
	assert.Equal(t, []interface{}{2, 6, 4, 8, 10}, arr.ToArray())
}

func TestArrayEListMoveObjectBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(5, 4)
	assert.Equal(t, []interface{}{2, 6, 8, 10, 4}, arr.ToArray())
}

func TestArrayEListMoveObjectEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(0, 4)
	assert.Equal(t, []interface{}{4, 2, 6, 8, 10}, arr.ToArray())
}

func TestArrayEListMoveObjectSame(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(1, 4)
	assert.Equal(t, []interface{}{2, 4, 6, 8, 10}, arr.ToArray())
}

func TestArrayEListMoveObjectInvalid(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	assert.Panics(t, func() { arr.MoveObject(1, 3) })
}

func TestArrayEListMoveInvalid(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	assert.Panics(t, func() { arr.Move(1, 7) })
}

func TestArrayEListRemoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(2)
	assert.Equal(t, []interface{}{4, 6}, arr.ToArray())
}

func TestArrayEListRemoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	assert.Equal(t, arr.Size(), 3)
	arr.Remove(6)
	assert.Equal(t, []interface{}{2, 4}, arr.ToArray())
}

func TestArrayEListRemoveMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 6})
	assert.Equal(t, arr.Size(), 4)
	arr.Remove(4)
	arr.Remove(6)
	assert.Equal(t, []interface{}{2, 6}, arr.ToArray())
}

func TestArrayEListRemoveInvalid(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	assert.False(t, arr.Remove(7))
}

func TestArrayEListRemoveAtInvalid(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	assert.Panics(t, func() { arr.RemoveAt(7) })
}

func TestArrayEListClear(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.Equal(t, arr.Size(), 2)
	arr.Clear()
	assert.True(t, arr.Empty())
}

func TestArrayEListEmptyTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	assert.False(t, arr.Empty())
}

func TestArrayEListEmptyFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{})
	assert.True(t, arr.Empty())
}

func TestArrayEListContainsFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.False(t, arr.Contains(4))
}

func TestArrayEListContainsTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	assert.True(t, arr.Contains(2))
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
	assert.Equal(t, []interface{}{2, 3, 5, 7}, arr2.ToArray())
	arr3 := NewArrayEList(nil)
	assert.False(t, arr2.AddAll(arr3))
}

func TestArrayEListAddUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{})
	assert.True(t, arr.Add(2))
	assert.True(t, arr.Add(5))
	assert.False(t, arr.Add(2))
}

func TestArrayEListInsertUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, 7})
	assert.Equal(t, arr.Insert(1, 2), true)
	assert.Equal(t, arr.Insert(2, 9), true)
	assert.Equal(t, arr.Insert(2, 3), false)
	assert.Equal(t, []interface{}{3, 2, 9, 5, 7}, arr.ToArray())
}

func TestArrayEListInsertAllUnique(t *testing.T) {
	arr := NewUniqueArrayEList([]interface{}{3, 5, -5, 7})
	arr2 := NewUniqueArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	assert.Equal(t, arr2.ToArray(), []interface{}{-3, 3, 5, 7, -5, -7})

	arr3 := NewArrayEList(nil)
	assert.False(t, arr2.InsertAll(1, arr3))
}
