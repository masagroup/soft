package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNotifierConstructor(t *testing.T) {
	assert.NotNil(t, NewNotifier())
}

func TestNotifierAccessors(t *testing.T) {
	var n ENotifier = NewNotifier()
	assert.True(t, n.EDeliver())

	n.ESetDeliver(false)
	assert.False(t, n.EDeliver())

	adapters := n.EAdapters()
	assert.True(t, adapters.Empty())
}
