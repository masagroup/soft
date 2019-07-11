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

func TestEClassInstance(t *testing.T) {
	eClass := newEClassExt()
	eClass.SetName("eClass")
	assert.Equal(t, eClass.GetName(), "eClass")
}

func TestEClassFeatures(t *testing.T) {
	eClass := newEClassExt()
	
	eAttribute1 := newEAttributeExt()
	eAttribute2 := newEAttributeExt()
	eReference1 := newEReferenceExt()
	eReference2 := newEReferenceExt()

	eFeatures := eClass.GetEStructuralFeatures()
	eFeatures.AddAll( NewImmutableEList([]interface{}{eAttribute1, eReference1, eAttribute2, eReference2 } ) )

	assert.Equal(t, eClass.GetEAttributes().ToArray(), []interface{}{eAttribute1, eAttribute2})
	assert.Equal(t, eClass.GetEReferences().ToArray(), []interface{}{eReference1, eReference2})

}

