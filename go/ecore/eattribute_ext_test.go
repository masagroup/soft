package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEAttributeEClass(t *testing.T) {
	assert.Equal(t, GetPackage().GetEAttribute(), GetFactory().CreateEAttribute().EClass())
}
