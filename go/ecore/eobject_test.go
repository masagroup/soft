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
	f := GetFactory()
	c := f.CreateEClass()
	f1 := f.CreateEAttribute()
	f2 := f.CreateEAttribute()
	o1 := f.CreateEOperation()
	c.GetEStructuralFeatures().AddAll(NewImmutableEList([]interface{}{f1, f2}))
	c.GetEOperations().Add(o1)
	assert.Equal(t, []interface{}{f1, f2, o1}, c.EContents().ToArray())
}
