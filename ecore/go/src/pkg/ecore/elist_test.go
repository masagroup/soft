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
	nb := arr.Get(1)
	if nb != 5 {
		check(t, nb, 5, "Get")
	}
}

func TestCount(t *testing.T) {
	arr := EArrayList{}
	arr.Add(3)
	arr.Add(5)
	arr.Add(7)
	nb := arr.Count()
	if nb != 3 {
		check(t, nb, 3, "Counter")
	}
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
