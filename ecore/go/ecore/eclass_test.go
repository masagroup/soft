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

func containsSubClass( eSuper *eClassExt , eClass *eClassExt ) bool {
	for _ , s := range eSuper.adapter.subClasses {
		if ( s == eClass ) {
			return true
		}
	}
	return false
}


func TestEClassSuperTypes(t *testing.T) {
	eClass := newEClassExt()
	eSuperClass := newEClassExt()
	eSuperClass2 := newEClassExt()
	

	eClass.GetESuperTypes().Add(eSuperClass)
	assert.True( t, containsSubClass( eSuperClass,eClass) )

	eClass.GetESuperTypes().Remove(eSuperClass)
	assert.False( t, containsSubClass( eSuperClass,eClass) )

	eClass.GetESuperTypes().AddAll( NewImmutableEList([]interface{}{eSuperClass,eSuperClass2}) )
	assert.True( t, containsSubClass( eSuperClass,eClass) )
	assert.True( t, containsSubClass( eSuperClass2,eClass) )
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



