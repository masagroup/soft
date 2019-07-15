package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
	
)

func TestEObjectEListInverseNoOpposite(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	mockOwner.On("EDeliver").Return( false )

	mockObject := &MockEObjectInternal{}
	list := NewEObjectEList(mockOwner, 1, -1, false, true, false, false, false)
	mockObject.On("EInverseAdd",mockOwner,-2,nil).Return(nil)
	
	assert.True(t, list.Add(mockObject))
	
	mockObject.On("EInverseRemove",mockOwner,-2,nil).Return(nil)
	assert.True(t, list.Remove(mockObject))

	mockObject.AssertExpectations(t)
}

func TestEObjectEListInverseOpposite(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	mockOwner.On("EDeliver").Return( false )
	
	mockObject := &MockEObjectInternal{}
	list := NewEObjectEList(mockOwner, 1, 2, false, true, true, false, false)
	
	mockObject.On("EInverseAdd",mockOwner,2,nil).Return(nil)
	assert.True(t, list.Add(mockObject))
	
	mockObject.On("EInverseRemove",mockOwner,2,nil).Return(nil)
	assert.True(t, list.Remove(mockObject))
	
	
	mockObject.AssertExpectations(t)
}
