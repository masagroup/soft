package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
	mock "github.com/stretchr/testify/mock"
)

func TestEObjectEList(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	mockObject := &MockEObject{}
	mockOwner.On("ENotify", mock.Anything).Return()
	{
		list := NewEObjectEList(mockOwner, 1, 2, false, false, false, false, false)
		assert.True(t, list.Add(mockObject))
		assert.Equal(t, list.Size(), 1)
		assert.Equal(t, list.Get(0), mockObject)
		assert.True(t, !list.Add(mockObject))
	}
	{
		mockOwner.On("EInverseAdd", mockObject, 2, nil).Return(NewNotificationChain())
		list := NewEObjectEList(mockOwner, 1, 2, false, true, true, false, false)
		assert.True(t, list.Add(mockObject))
		assert.Equal(t, list.Size(), 1)
		assert.Equal(t, list.Get(0), mockObject)
		assert.True(t, !list.Add(mockObject))
	}
}
