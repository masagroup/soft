package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEListIterator(t *testing.T) {
	mockList := &MockEList{}
	mockList.On("Size").Return(3)
	for i := 0; i < 3; i++ {
		mockList.On("Get", i).Return(i)
	}
	it := &listIterator{list: mockList}
	for i := 0; it.HasNext(); i++ {
		assert.Equal(t, i, it.Next())
	}
	assert.Panics(t, func() { it.Next() })
}
