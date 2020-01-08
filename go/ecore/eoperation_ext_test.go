package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestOperationIsOverrideOf(t *testing.T) {
	class1 := newEClassExt()
	class2 := newEClassExt()
	operation1 := newEOperationExt()
	operation2 := newEOperationExt()
	mockParameter1 := newEParameterImpl()
	mockParameter2 := newEParameterImpl()
	mockType := &MockEClassifier{}
	mockType.On("EIsProxy").Return(false)
	mockOtherType := &MockEClassifier{}
	mockOtherType.On("EIsProxy").Return(false)
	class1.GetEOperations().Add(operation1)
	class2.GetEOperations().Add(operation2)

	// no relationship
	assert.False(t, operation1.IsOverrideOf(operation2))
	// different names and relationship
	class1.GetESuperTypes().Add(class2)
	operation1.SetName("op1")
	operation1.SetName("op2")
	assert.False(t, operation1.IsOverrideOf(operation2))
	// same names + relationship / different parameters
	operation1.SetName("op")
	operation2.SetName("op")
	operation1.GetEParameters().Add(mockParameter1)
	operation2.GetEParameters().Add(mockParameter2)
	mockParameter1.SetEType(mockType)
	mockParameter2.SetEType(mockOtherType)
	assert.False(t, operation1.IsOverrideOf(operation2))
	// all same
	mockParameter1.SetEType(mockType)
	mockParameter2.SetEType(mockType)
	assert.True(t, operation1.IsOverrideOf(operation2))
}

func TestEOperationEClass(t *testing.T) {
	assert.Equal(t, GetPackage().GetEOperation(), GetFactory().CreateEOperation().EClass())
}
