package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNotificationChainAdd(t *testing.T) {
	chain := NewNotificationChain()
	assert.False(t, chain.Add(nil))

	mockNotif1 := &MockENotification{}
	assert.True(t, chain.Add(mockNotif1))

	mockNotif2 := &MockENotification{}
	mockNotif1.On("Merge", mockNotif2).Return(true).Once()
	assert.False(t, chain.Add(mockNotif2))

	mockNotif3 := &MockENotification{}
	mockNotif1.On("Merge", mockNotif3).Return(false).Once()
	assert.True(t, chain.Add(mockNotif3))

	mockNotif1.AssertExpectations(t)
	mockNotif2.AssertExpectations(t)
	mockNotif3.AssertExpectations(t)
}
