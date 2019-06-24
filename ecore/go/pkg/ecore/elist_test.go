package ecore

import (
	"testing"
)

func check(t *testing.T, result, expectation interface{}, currentTest string) {
	if result != expectation {
		t.Error("Incorrect ", currentTest, ", got: ", result, ", expected: ", expectation)
	}
}

func checkNot(t *testing.T, result, expectation interface{}, currentTest string) {
	if result == expectation {
		t.Error("Incorrect ", currentTest, ", got: ", result, ", expected: ", expectation)
	}
}

func TestGet(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	check(t, arr.Get(0), 3, "Get [0]")
	check(t, arr.Get(1), 5, "Get [1]")
	check(t, arr.Get(2), 7, "Get [2]")
}

func TestSize(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	check(t, arr.Size(), 3, "Size")
}

func TestAddAll(t *testing.T) {
	tmp := NewArrayEList([]interface{}{3, 5, 7})
	var arr EList
	arr = tmp
	arr2 := NewArrayEList([]interface{}{})
	arr2.AddAll(arr)
	check(t, arr2.Size(), 3, "Add all (size check)")
	check(t, arr2.Get(0), 3, "Add all (get [0] check)")
	check(t, arr2.Get(1), 5, "Add all (get [1] check)")
	check(t, arr2.Get(2), 7, "Add all (get [2] check)")
}

func TestInsertPrepend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	check(t, arr.Insert(0, 2), true, "Insert begin (insert check)")
	check(t, arr.Size(), 2, "Insert begin (size check)")
	check(t, arr.Get(0), 2, "Insert begin (get [0] check)")
	check(t, arr.Get(1), 3, "Insert begin (get [1] check)")
}

func TestInsertAppend(t *testing.T) {
	arr := NewArrayEList([]interface{}{3})
	check(t, arr.Insert(1, 2), true, "Insert end (insert check)")
	check(t, arr.Size(), 2, "Insert end (size check)")
	check(t, arr.Get(0), 3, "Insert end (get [0] check)")
	check(t, arr.Get(1), 2, "Insert end (get [1] check)")
}

func TestInsertMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5, 7})
	check(t, arr.Insert(1, 2), true, "Insert middle (insert check)")
	check(t, arr.Insert(2, 3), true, "Insert middle (insert check)")
	check(t, arr.Size(), 5, "Insert middle (size check)")
	check(t, arr.Get(0), 3, "Insert middle (get [0] check)")
	check(t, arr.Get(1), 2, "Insert middle (get [1] check)")
	check(t, arr.Get(2), 3, "Insert middle (get [2] check)")
	check(t, arr.Get(3), 5, "Insert middle (get [3] check)")
	check(t, arr.Get(4), 7, "Insert middle (get [4] check)")
}

func TestInsertAll(t *testing.T) {
	tmp := NewArrayEList([]interface{}{3, 5, 7})
	var arr EList
	arr = tmp
	arr2 := NewArrayEList([]interface{}{-3, -5, -7})
	arr2.InsertAll(1, arr)
	check(t, arr2.Size(), 6, "Insert all (size check)")
	check(t, arr2.Get(0), -3, "Insert all (get [0] check)")
	check(t, arr2.Get(1), 3, "Insert all (get [1] check)")
	check(t, arr2.Get(2), 5, "Insert all (get [2] check)")
	check(t, arr2.Get(3), 7, "Insert all (get [3] check)")
	check(t, arr2.Get(4), -5, "Insert all (get [4] check)")
	check(t, arr2.Get(5), -7, "Insert all (get [5] check)")
}

func TestMoveAfter(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(3, 4)
	check(t, arr.Size(), 5, "Move after (size check)")
	check(t, arr.Get(0), 2, "Move after (get[0] check)")
	check(t, arr.Get(1), 6, "Move after (get[1] check)")
	check(t, arr.Get(2), 4, "Move after (get[2] check)")
	check(t, arr.Get(3), 8, "Move after (get[3] check)")
	check(t, arr.Get(4), 10, "Move after (get[4] check)")
}

func TestMoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(5, 4)
	check(t, arr.Size(), 5, "Move begin (size check)")
	check(t, arr.Get(0), 2, "Move begin (get[0] check)")
	check(t, arr.Get(1), 6, "Move begin (get[1] check)")
	check(t, arr.Get(2), 8, "Move begin  (get[2] check)")
	check(t, arr.Get(3), 10, "Move begin (get[3] check)")
	check(t, arr.Get(4), 4, "Move begin (get[4] check)")
}

func TestMoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(0, 4)
	check(t, arr.Size(), 5, "Move end (size check)")
	check(t, arr.Get(0), 4, "Move end (get[0] check)")
	check(t, arr.Get(1), 2, "Move end (get[1] check)")
	check(t, arr.Get(2), 6, "Move end (get[2] check)")
	check(t, arr.Get(3), 8, "Move end (get[3] check)")
	check(t, arr.Get(4), 10, "Move end (get[4] check)")
}

func TestMoveSame(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 8, 10})
	arr.MoveObject(1, 4)
	check(t, arr.Size(), 5, "Move same (size check)")
	check(t, arr.Get(0), 2, "Move same (get[0] check)")
	check(t, arr.Get(1), 4, "Move same (get[1] check)")
	check(t, arr.Get(2), 6, "Move same (get[2] check)")
	check(t, arr.Get(3), 8, "Move same (get[3] check)")
	check(t, arr.Get(4), 10, "Move same (get[4] check)")
}

func TestRemoveBegin(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	check(t, arr.Size(), 3, "Remove begin (size check)")
	arr.Remove(2)
	check(t, arr.Size(), 2, "Remove begin (size check)")
	check(t, arr.Get(0), 4, "Remove begin (get[0] check)")
	check(t, arr.Get(1), 6, "Remove begin (get[1] check)")
}

func TestRemoveEnd(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6})
	check(t, arr.Size(), 3, "Remove end (size check)")
	arr.Remove(6)
	check(t, arr.Size(), 2, "Remove end (size check)")
	check(t, arr.Get(0), 2, "Remove end (get[0] check)")
	check(t, arr.Get(1), 4, "Remove end (get[1] check)")
}

func TestRemoveMiddle(t *testing.T) {
	arr := NewArrayEList([]interface{}{2, 4, 6, 6})
	check(t, arr.Size(), 4, "Remove middle (size check)")
	arr.Remove(4)
	arr.Remove(6)
	check(t, arr.Size(), 2, "Remove middle (size check)")
	check(t, arr.Get(0), 2, "Remove middle (get[0] check)")
	check(t, arr.Get(1), 6, "Remove middle (get[1] check)")
}

func TestClear(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	check(t, arr.Size(), 2, "Clear")
	arr.Clear()
	check(t, arr.Size(), 0, "Clear")
}

func TestEmptyTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{3, 5})
	check(t, arr.Empty(), false, "Empty")
}

func TestEmptyFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{})
	check(t, arr.Empty(), true, "Empty")
}

func TestContainsFalse(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	check(t, arr.Contains(4), false, "Contains")
}

func TestContainsTrue(t *testing.T) {
	arr := NewArrayEList([]interface{}{2})
	check(t, arr.Contains(2), true, "Contains")
}

func TestIterate(t *testing.T) {
	arr := NewArrayEList([]interface{}{0, 2, 4})
	i := 0
	for it := arr.Iterate(); it.Next(); {
		check(t, it.Value(), i, "Iterate")
		i += 2
	}
	check(t, 6, i, "Iterate")
}

func TestIterateImmutable(t *testing.T) {
	var iDatas []interface{}
	iDatas = append(iDatas, 0, 2, 4, 6)
	arr := NewImmutableEList(iDatas)
	i := 0
	for it := arr.Iterate(); it.Next(); {
		check(t, it.Value(), i, "Iterate immutable")
		i += 2
	}
	check(t, 8, i, "Iterate immutable")
}
