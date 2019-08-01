package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestETreeIteratorWithRoot(t *testing.T) {
	emptyList := NewImmutableEList(nil)
	mockObject := new(MockEObject)
	it := newTreeIterator(mockObject, true, func(i interface{}) EIterator {
		return emptyList.Iterator()
	})
	assert.True(t, it.HasNext())
	assert.Equal(t, mockObject, it.Next())
	assert.False(t, it.HasNext())
}

func TestEAllContentsIteratorEmpty(t *testing.T) {
	emptyList := NewImmutableEList(nil)
	mockObject := new(MockEObject)
	mockObject.On("EContents").Return(emptyList)
	it := newEAllContentsIterator(mockObject)
	assert.False(t, it.HasNext())
}

func TestEAllContentsIteratorNotEmpty(t *testing.T) {
	emptyList := NewImmutableEList(nil)
	mockObject := new(MockEObject)
	mockChild1 := new(MockEObject)
	mockGrandChild1 := new(MockEObject)
	mockGrandChild2 := new(MockEObject)
	mockChild2 := new(MockEObject)
	mockObject.On("EContents").Return(NewImmutableEList([]interface{}{mockChild1, mockChild2}))
	mockChild1.On("EContents").Return(NewImmutableEList([]interface{}{mockGrandChild1, mockGrandChild2}))
	mockGrandChild1.On("EContents").Return(emptyList)
	mockGrandChild2.On("EContents").Return(emptyList)
	mockChild2.On("EContents").Return(emptyList)

	var result []interface{}
	for it := newEAllContentsIterator(mockObject); it.HasNext(); {
		result = append(result, it.Next())
	}
	assert.Equal(t, []interface{}{mockChild1, mockGrandChild1, mockGrandChild2, mockChild2}, result)
}
