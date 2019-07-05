package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
	mock "github.com/stretchr/testify/mock"
)

type eObjectEListExample struct {
	*eObjectEList
	c bool
	i bool
	o bool
	p bool
	u bool
}

func (e *eObjectEListExample) containment() bool {
	return e.c
}

func (e *eObjectEListExample) inverse() bool {
	return e.i
}

func (e *eObjectEListExample) opposite() bool {
	return e.o
}

func (e *eObjectEListExample) proxies() bool {
	return e.p
}

func (e *eObjectEListExample) unset() bool {
	return e.u
}

func newEObjectEListExample(owner EObjectInternal, id, reverseID int, c, i, o, u, p bool) *eObjectEListExample {
	tmp := &eObjectEListExample{
		eObjectEList: NewEObjectEList(owner, 1, 2),
	}
	tmp.internal = tmp
	return tmp
}

func TestEObjectEList(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	mockObject := &MockEObject{}
	mockOwner.On("ENotify", mock.Anything).Return()
	{
		list := newEObjectEListExample(mockOwner, 1, 2, false, false, false, false, false)
		assert.True(t, list.Add(mockObject))
		assert.Equal(t, list.Size(), 1)
		assert.Equal(t, list.Get(0), mockObject)
		assert.True(t, !list.Add(mockObject))
	}
	{
		mockOwner.On("EInverseAdd", mockOwner, 2, nil).Once().Return(nil)
		list := newEObjectEListExample(mockOwner, 1, 2, false, true, true, false, false)
		assert.True(t, list.Add(mockObject))
		assert.Equal(t, list.Size(), 1)
		assert.Equal(t, list.Get(0), mockObject)
		assert.True(t, !list.Add(mockObject))
	}
}
