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
<<<<<<< HEAD
	arr := &testingObjectEList{}
	arr.eObjectEListImpl = NewEObjectEList(NewBasicEObject(), -1, -1)

	adapter := &customTestAdapterMerge{Adapter: *NewAdapter()}
	arr.owner.EAdapters().Add(adapter)
=======
	test := &testingObjectEList{}
	test.eObjectEListImpl = NewEObjectEList()
	test.eObjectEListImpl.internal = test
	fmt.Println(test.resolve())
>>>>>>> [Go] Add EObjectEList inheritance
}
