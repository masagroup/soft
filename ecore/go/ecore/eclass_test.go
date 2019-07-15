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

func TestEClassFeaturesAdd(t *testing.T) {
	eClass := newEClassExt()
	eAttribute := newEAttributeExt()
	assert.Equal(t , -1 , eAttribute.GetFeatureID() )

	eFeatures := eClass.GetEStructuralFeatures()
	eFeatures.Add( eAttribute )

	assert.Equal(t , 1 , eClass.GetFeatureCount() )
	assert.Equal(t , 0 , eAttribute.GetFeatureID() )
}


func TestEClassFeaturesGetters(t *testing.T) {
	eClass := newEClassExt()
	
	eAttribute1 := newEAttributeExt()
	eAttribute2 := newEAttributeExt()
	eReference1 := newEReferenceExt()
	eReference2 := newEReferenceExt()

	eFeatures := eClass.GetEStructuralFeatures()
	eFeatures.AddAll( NewImmutableEList([]interface{}{eAttribute1, eReference1, eAttribute2, eReference2 } ) )

	// feature ids
	assert.Equal(t, 4, eClass.GetFeatureCount() )
	assert.Equal(t, eAttribute1, eClass.GetEStructuralFeature(0) )
	assert.Equal(t, eReference1, eClass.GetEStructuralFeature(1) )
	assert.Equal(t, eAttribute2, eClass.GetEStructuralFeature(2) )
	assert.Equal(t, eReference2, eClass.GetEStructuralFeature(3) )
	
	assert.Equal(t, 0, eAttribute1.GetFeatureID() )
	assert.Equal(t, 2, eAttribute2.GetFeatureID() )
	assert.Equal(t, 1, eReference1.GetFeatureID() )
	assert.Equal(t, 3, eReference2.GetFeatureID() )

	// collections
	assert.Equal(t, eClass.GetEAllStructuralFeatures().ToArray(), []interface{}{eAttribute1, eReference1, eAttribute2, eReference2})
	assert.Equal(t, eClass.GetEAllAttributes().ToArray(), []interface{}{eAttribute1, eAttribute2})
	assert.Equal(t, eClass.GetEAllReferences().ToArray(), []interface{}{eReference1, eReference2})
	assert.Equal(t, eClass.GetEAttributes().ToArray(), []interface{}{eAttribute1, eAttribute2})
	assert.Equal(t, eClass.GetEReferences().ToArray(), []interface{}{eReference1, eReference2})
	
	eAttribute3 := newEAttributeExt()
	eFeatures.Add( eAttribute3 )
	assert.Equal(t, eClass.GetEAllAttributes().ToArray(), []interface{}{eAttribute1, eAttribute2,eAttribute3})
	assert.Equal(t, eClass.GetEAttributes().ToArray(), []interface{}{eAttribute1, eAttribute2,eAttribute3})
}


func TestEClassOperationsGetters(t *testing.T) {
	eClass := newEClassExt()
	
	eOperation1 := newEOperationExt()
	eOperation2 := newEOperationExt()
	

	eOperations := eClass.GetEOperations()
	eOperations.AddAll( NewImmutableEList([]interface{}{eOperation1, eOperation2 } ) )

	// feature ids
	assert.Equal(t, 2, eClass.GetOperationCount() )
	assert.Equal(t, eOperation1, eClass.GetEOperation(0) )
	assert.Equal(t, eOperation2, eClass.GetEOperation(1) )

	assert.Equal(t, 0, eOperation1.GetOperationID() )
	assert.Equal(t, 1, eOperation2.GetOperationID() )

	// collections
	assert.Equal(t, eClass.GetEAllOperations().ToArray(), []interface{}{eOperation1, eOperation2})
	assert.Equal(t, eClass.GetEOperations().ToArray(), []interface{}{eOperation1, eOperation2})
	
	eOperation3 := newEOperationExt()
	eOperations.Add( eOperation3 )
	assert.Equal(t, eClass.GetEAllOperations().ToArray(), []interface{}{eOperation1, eOperation2, eOperation3 })
	assert.Equal(t, eClass.GetEOperations().ToArray(), []interface{}{eOperation1, eOperation2, eOperation3 })
}
