package ecore

import (
	"fmt"
	"testing"
)

type testingObjectEList struct {
	*eObjectEListImpl
}

func (arr *testingObjectEList) containment() bool {
	return true
}

func (arr *testingObjectEList) inverse() bool {
	return false
}

func (arr *testingObjectEList) opposite() bool {
	return false
}

func (arr *testingObjectEList) proxies() bool {
	return false
}

func (arr *testingObjectEList) unset() bool {
	return false
}

func TestEObjectEList(t *testing.T) {
	test := &testingObjectEList{}
	test.eObjectEListImpl = NewEObjectEList()
	test.eObjectEListImpl.internal = test
	fmt.Println(test.resolve())
}
