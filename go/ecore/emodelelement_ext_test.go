package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEModelElementGetAnnotation(t *testing.T) {
	m := newEModelElementExt()
	a1 := new(MockEAnnotation)
	a1.On("GetSource").Return("a1")
	a2 := new(MockEAnnotation)
	a2.On("GetSource").Return("a2")
	m.GetEAnnotations().Add( a1 )
	m.GetEAnnotations().Add( a2 )
	assert.Equal(t, a2, m.GetEAnnotation("a2") )
	assert.Equal(t, nil, m.GetEAnnotation("a") )
}