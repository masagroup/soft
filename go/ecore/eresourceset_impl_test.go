package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEResourceSetConstructor(t *testing.T) {
	rs := NewEResourceSetImpl()
	assert.Nil(t, rs.GetURIResourceMap())
}

func TestEResourceSetResourcesWithMock(t *testing.T) {
	rs := NewEResourceSetImpl()
	r := new(MockEResourceInternal)
	r.On("basicSetResourceSet", rs, nil).Return(nil)
	rs.GetResources().Add(r)
}

func TestEResourceSetResourcesNoMock(t *testing.T) {
	rs := NewEResourceSetImpl()
	r := NewEResourceImpl()

	rs.GetResources().Add(r)
	assert.Equal(t, rs, r.GetResourceSet())

	rs.GetResources().Remove(r)
	assert.Equal(t, nil, r.GetResourceSet())
}
