package ecore

import (
	"testing"
)

func check(t *testing.T, result, expectation interface{}, currentTest string) {
	if result != expectation {
		t.Error("Incorrect ", currentTest, ", got: ", result, ", expected: ", expectation)
	}
}

func TestGet(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	arr.Add(5)
	arr.Add(7)
	check(t, arr.Get(0), 3, "Get [0]")
	check(t, arr.Get(1), 5, "Get [1]")
	check(t, arr.Get(2), 7, "Get [2]")
}

func TestSize(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	arr.Add(5)
	arr.Add(7)
	check(t, arr.Size(), 3, "Size")
}

func TestAddAll(t *testing.T) {
	tmp := EArrayList{}
	var arr EList
	arr = &tmp
	arr.Add(3)
	arr.Add(5)
	arr.Add(7)
	arr2 := EArrayList{}
	arr2.AddAll(arr)
	check(t, arr2.Size(), 3, "Add all (size check)")
	check(t, arr2.Get(0), 3, "Add all (get [0] check)")
	check(t, arr2.Get(1), 5, "Add all (get [1] check)")
	check(t, arr2.Get(2), 7, "Add all (get [2] check)")
}

func TestInsertBad(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(-1, 2), false, "Insert error (insert check)")
	check(t, arr.Size(), 1, "Insert error (size check)")
}

func TestInsertBad2(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(2, 2), false, "Insert error (insert check)")
	check(t, arr.Size(), 1, "Insert error (size check)")
}

func TestInsertPrepend(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(0, 2), true, "Insert begin (insert check)")
	check(t, arr.Size(), 2, "Insert begin (size check)")
	check(t, arr.Get(0), 2, "Insert begin (get [0] check)")
	check(t, arr.Get(1), 3, "Insert begin (get [1] check)")
}

func TestInsertAppend(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(1, 2), true, "Insert end (insert check)")
	check(t, arr.Size(), 2, "Insert end (size check)")
	check(t, arr.Get(0), 3, "Insert end (get [0] check)")
	check(t, arr.Get(1), 2, "Insert end (get [1] check)")
}

func TestInsertMiddle(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	arr.Add(5)
	arr.Add(7)
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
	tmp := EArrayList{}
	var arr EList
	arr = &tmp
	arr.Add(3)
	arr.Add(5)
	arr.Add(7)
	arr2 := EArrayList{}
	arr2.Add(-3)
	arr2.Add(-5)
	arr2.Add(-7)
	arr2.InsertAll(1, arr)
	check(t, arr2.Size(), 6, "Insert all (size check)")
	check(t, arr2.Get(0), -3, "Insert all (get [0])")
	check(t, arr2.Get(1), 3, "Insert all (get [1])")
	check(t, arr2.Get(2), 5, "Insert all (get [2])")
	check(t, arr2.Get(3), 7, "Insert all (get [3])")
	check(t, arr2.Get(4), -5, "Insert all (get [4])")
	check(t, arr2.Get(5), -7, "Insert all (get [5])")
}

func TestIterate(t *testing.T) {
	arr := EArrayList{}
	arr.Add(0)
	arr.Add(2)
	arr.Add(4)
	i := 0
	for val := range arr.Iterate() {
		check(t, val, i, "Iterate")
		i += 2
	}
}
