package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestAdapterAccessors(t *testing.T) {
	adapter := NewAdapter()
	assert.Equal(t, nil, adapter.GetTarget())

	mockNotifier := new(MockENotifier)
	adapter.SetTarget(mockNotifier)
	assert.Equal(t, mockNotifier, adapter.GetTarget())
}
