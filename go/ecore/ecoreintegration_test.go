package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestECoreEClasses(t *testing.T) {
	f := GetFactory()
	p := GetPackage()
	assert.Equal(t, p.GetEClass(), f.CreateEClass().EClass())
	assert.Equal(t, p.GetEAttribute(), f.CreateEAttribute().EClass())
	assert.Equal(t, p.GetEOperation(), f.CreateEOperation().EClass())
	assert.Equal(t, p.GetEDataType(), f.CreateEDataType().EClass())
}

func TestECoreEContents(t *testing.T) {
	f := GetFactory()
	c := f.CreateEClass()
	f1 := f.CreateEAttribute()
	f1.SetName("f1")
	f2 := f.CreateEAttribute()
	f2.SetName("f2")
	c.GetEStructuralFeatures().AddAll(NewImmutableEList([]interface{}{f1, f2}))
	o1 := f.CreateEOperation()
	o1.SetName("o1")
	c.GetEOperations().Add(o1)
	assert.Equal(t, []interface{}{f1, f2, o1}, c.EContents().ToArray())
}
