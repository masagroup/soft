package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEClassInstance(t *testing.T) {
	eClass := newEClassImpl()
	eClass.SetName("eClass")
	assert.Equal(t, eClass.GetName(), "eClass")
}
