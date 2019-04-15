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
	check(t, arr.Get(1), 5, "Get")
}

func TestSize(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	arr.Add(5)
	arr.Add(7)
	check(t, arr.Size(), 3, "Sizeer")
}

func TestInsertBad(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(-1, 2), false, "Insert error (insert check)")
	check(t, arr.Size(), 1, "Insert error (Size check)")
}

func TestInsertBad2(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(2, 2), false, "Insert error (insert check)")
	check(t, arr.Size(), 1, "Insert error (Size check)")
}

func TestInsertPrepend(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(0, 2), true, "Insert begin (insert check)")
	check(t, arr.Size(), 2, "Insert begin (Size check)")
	check(t, arr.Get(0), 2, "Insert begin (get [0] check)")
	check(t, arr.Get(1), 3, "Insert begin (get [1] check)")
}

func TestInsertAppend(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	check(t, arr.Insert(1, 2), true, "Insert end (insert check)")
	check(t, arr.Size(), 2, "Insert end (Size check)")
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
	check(t, arr.Size(), 5, "Insert middle (Size check)")
	check(t, arr.Get(0), 3, "Insert middle (get [0] check)")
	check(t, arr.Get(1), 2, "Insert middle (get [1] check)")
	check(t, arr.Get(2), 3, "Insert middle (get [2] check)")
	check(t, arr.Get(3), 5, "Insert middle (get [3] check)")
	check(t, arr.Get(4), 7, "Insert middle (get [4] check)")
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
