package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEObjectEListAccessors(t *testing.T) {
	{
		list := NewEObjectEList(nil, 1, -1, false, true, false, false, false)
		assert.Equal(t, nil, list.GetNotifier())
		assert.Equal(t, nil, list.GetFeature())
		assert.Equal(t, 1, list.GetFeatureID())
	}
	{
		mockOwner := &MockEObjectInternal{}
		list := NewEObjectEList(mockOwner, 1, -1, false, true, false, false, false)
		assert.Equal(t, mockOwner, list.GetNotifier())
		assert.Equal(t, 1, list.GetFeatureID())
		mockClass := &MockEClass{}
		mockFeature := &MockEStructuralFeature{}
		mockClass.On("GetEStructuralFeature", 1).Return(mockFeature)
		mockOwner.On("EClass").Return(mockClass)
		assert.Equal(t, mockFeature, list.GetFeature())
		mockOwner.AssertExpectations(t)
		mockClass.AssertExpectations(t)
		mockFeature.AssertExpectations(t)
		mockClass.AssertExpectations(t)
	}
}

func TestEObjectEListInverseNoOpposite(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	mockOwner.On("EDeliver").Return(false)

	mockObject := &MockEObjectInternal{}
	list := NewEObjectEList(mockOwner, 1, -1, false, true, false, false, false)
	mockObject.On("EInverseAdd", mockOwner, -2, nil).Return(nil)

	assert.True(t, list.Add(mockObject))

	mockObject.On("EInverseRemove", mockOwner, -2, nil).Return(nil)
	assert.True(t, list.Remove(mockObject))

	mockObject.AssertExpectations(t)
}

func TestEObjectEListInverseOpposite(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	mockOwner.On("EDeliver").Return(false)

	mockObject := &MockEObjectInternal{}
	list := NewEObjectEList(mockOwner, 1, 2, false, true, true, false, false)

	mockObject.On("EInverseAdd", mockOwner, 2, nil).Return(nil)
	assert.True(t, list.Add(mockObject))

	mockObject.On("EInverseRemove", mockOwner, 2, nil).Return(nil)
	assert.True(t, list.Remove(mockObject))

	mockObject.AssertExpectations(t)
}
