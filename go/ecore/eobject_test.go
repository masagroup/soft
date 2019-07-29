// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEObjectEContents(t *testing.T) {
	factory := GetFactory()
	c := factory.CreateEClass()
	f1 := factory.CreateEAttribute()
	f2 := factory.CreateEAttribute()
	o1 := factory.CreateEOperation()
	c.GetEStructuralFeatures().AddAll(NewImmutableEList([]interface{}{f1, f2}))
	c.GetEOperations().Add(o1)
	assert.Equal(t, []interface{}{f1, f2, o1}, c.EContents().ToArray())
}

func TestEObjectEContainingFeature(t *testing.T) {
	factory := GetFactory()
	c := factory.CreateEClass()
	f := factory.CreateEAttribute()
	o := factory.CreateEOperation()
	c.GetEStructuralFeatures().Add(f)
	c.GetEOperations().Add(o)
	assert.Equal(t, GetPackage().GetEClass_EStructuralFeatures(), f.EContainingFeature())
	assert.Equal(t, GetPackage().GetEClass_EOperations(), o.EContainingFeature())
}
