package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

type testingObjectEList {
	ArrayEList
}

func (arr *testingObjectEList) containment() bool {
	return false
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

func TestFactoryChar(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertECharToString(nil, factory.createECharFromString(nil, "e")), "e")
}
