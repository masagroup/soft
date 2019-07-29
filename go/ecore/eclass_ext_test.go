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

func containsSubClass(eSuper *eClassExt, eClass *eClassExt) bool {
	for _, s := range eSuper.adapter.subClasses {
		if s == eClass {
			return true
		}
	}
	return false
}

func TestEClassSuperTypes(t *testing.T) {
	eClass := newEClassExt()
	eSuperClass := newEClassExt()
	eSuperClass2 := newEClassExt()
	eSuperClass3 := newEClassExt()
	eSuperSuperClass := newEClassExt()

	// build class hierarchy
	eClass.GetESuperTypes().Add(eSuperClass)
	eSuperClass.GetESuperTypes().Add(eSuperSuperClass)

	// test super types getters
	assert.Equal(t, []interface{}{eSuperClass}, eClass.GetESuperTypes().ToArray())
	assert.Equal(t, []interface{}{eSuperSuperClass, eSuperClass}, eClass.GetEAllSuperTypes().ToArray())
	assert.True(t, containsSubClass(eSuperClass, eClass))

	// remove super class
	eClass.GetESuperTypes().Remove(eSuperClass)
	assert.False(t, containsSubClass(eSuperClass, eClass))

	// add many super classes
	eClass.GetESuperTypes().AddAll(NewImmutableEList([]interface{}{eSuperClass, eSuperClass2}))
	assert.True(t, containsSubClass(eSuperClass, eClass))
	assert.True(t, containsSubClass(eSuperClass2, eClass))

	// set super classes
	eClass.GetESuperTypes().Set(1, eSuperClass3)
	assert.True(t, containsSubClass(eSuperClass, eClass))
	assert.True(t, containsSubClass(eSuperClass3, eClass))

	// remove many
}

func TestEClassFeaturesAdd(t *testing.T) {
	eClass := newEClassExt()
	eAttribute := newEAttributeExt()
	assert.Equal(t, -1, eAttribute.GetFeatureID())

	eFeatures := eClass.GetEStructuralFeatures()
	eFeatures.Add(eAttribute)

	assert.Equal(t, 1, eClass.GetFeatureCount())
	assert.Equal(t, 0, eAttribute.GetFeatureID())
	assert.Equal(t, eClass, eAttribute.GetEContainingClass())
}

func TestEClassFeaturesGetters(t *testing.T) {
	eClass := newEClassExt()

	eAttribute1 := newEAttributeExt()
	eAttribute2 := newEAttributeExt()
	eReference1 := newEReferenceExt()
	eReference2 := newEReferenceExt()

	eFeatures := eClass.GetEStructuralFeatures()
	eFeatures.AddAll(NewImmutableEList([]interface{}{eAttribute1, eReference1, eAttribute2, eReference2}))

	// feature ids
	assert.Equal(t, 4, eClass.GetFeatureCount())
	assert.Equal(t, eAttribute1, eClass.GetEStructuralFeature(0))
	assert.Equal(t, eReference1, eClass.GetEStructuralFeature(1))
	assert.Equal(t, eAttribute2, eClass.GetEStructuralFeature(2))
	assert.Equal(t, eReference2, eClass.GetEStructuralFeature(3))
	assert.Equal(t, nil, eClass.GetEStructuralFeature(4))
	assert.Equal(t, 0, eAttribute1.GetFeatureID())
	assert.Equal(t, 2, eAttribute2.GetFeatureID())
	assert.Equal(t, 1, eReference1.GetFeatureID())
	assert.Equal(t, 3, eReference2.GetFeatureID())
	assert.Equal(t, 0, eClass.GetFeatureID(eAttribute1))
	assert.Equal(t, 2, eClass.GetFeatureID(eAttribute2))
	assert.Equal(t, 1, eClass.GetFeatureID(eReference1))
	assert.Equal(t, 3, eClass.GetFeatureID(eReference2))

	// collections
	assert.Equal(t, []interface{}{eAttribute1, eReference1, eAttribute2, eReference2}, eClass.GetEAllStructuralFeatures().ToArray())
	assert.Equal(t, []interface{}{eAttribute1, eAttribute2}, eClass.GetEAllAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference1, eReference2}, eClass.GetEAllReferences().ToArray())
	assert.Equal(t, []interface{}{eAttribute1, eAttribute2}, eClass.GetEAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference1, eReference2}, eClass.GetEReferences().ToArray())

	// insert another attribute front
	eAttribute3 := newEAttributeExt()
	eFeatures.Insert(0, eAttribute3)
	assert.Equal(t, []interface{}{eAttribute3, eAttribute1, eAttribute2}, eClass.GetEAllAttributes().ToArray())
	assert.Equal(t, []interface{}{eAttribute3, eAttribute1, eAttribute2}, eClass.GetEAttributes().ToArray())

	// feature ids
	assert.Equal(t, 5, eClass.GetFeatureCount())
	assert.Equal(t, eAttribute3, eClass.GetEStructuralFeature(0))
	assert.Equal(t, eAttribute1, eClass.GetEStructuralFeature(1))
	assert.Equal(t, eReference1, eClass.GetEStructuralFeature(2))
	assert.Equal(t, eAttribute2, eClass.GetEStructuralFeature(3))
	assert.Equal(t, eReference2, eClass.GetEStructuralFeature(4))
	assert.Equal(t, 0, eAttribute3.GetFeatureID())
	assert.Equal(t, 1, eAttribute1.GetFeatureID())
	assert.Equal(t, 3, eAttribute2.GetFeatureID())
	assert.Equal(t, 2, eReference1.GetFeatureID())
	assert.Equal(t, 4, eReference2.GetFeatureID())

}

func TestEClassFeaturesGettersWithSuperType(t *testing.T) {
	eClass := newEClassExt()
	eSuperClass := newEClassExt()
	eClass.GetESuperTypes().Add(eSuperClass)

	eAttribute1 := newEAttributeExt()
	eReference1 := newEReferenceExt()
	eAttribute2 := newEAttributeExt()
	eReference2 := newEReferenceExt()
	eClass.GetEStructuralFeatures().AddAll(NewImmutableEList([]interface{}{eAttribute1, eReference1}))
	eSuperClass.GetEStructuralFeatures().AddAll(NewImmutableEList([]interface{}{eAttribute2, eReference2}))

	// collections
	assert.Equal(t, []interface{}{eAttribute2, eReference2}, eSuperClass.GetEAllStructuralFeatures().ToArray())
	assert.Equal(t, []interface{}{eAttribute2}, eSuperClass.GetEAllAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference2}, eSuperClass.GetEAllReferences().ToArray())
	assert.Equal(t, []interface{}{eAttribute2}, eSuperClass.GetEAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference2}, eSuperClass.GetEReferences().ToArray())

	assert.Equal(t, []interface{}{eAttribute2, eReference2, eAttribute1, eReference1}, eClass.GetEAllStructuralFeatures().ToArray())
	assert.Equal(t, []interface{}{eAttribute2, eAttribute1}, eClass.GetEAllAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference2, eReference1}, eClass.GetEAllReferences().ToArray())
	assert.Equal(t, []interface{}{eAttribute1}, eClass.GetEAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference1}, eClass.GetEReferences().ToArray())

	// now remove super type
	eClass.GetESuperTypes().Remove(eSuperClass)

	assert.Equal(t, []interface{}{eAttribute1, eReference1}, eClass.GetEAllStructuralFeatures().ToArray())
	assert.Equal(t, []interface{}{eAttribute1}, eClass.GetEAllAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference1}, eClass.GetEAllReferences().ToArray())
	assert.Equal(t, []interface{}{eAttribute1}, eClass.GetEAttributes().ToArray())
	assert.Equal(t, []interface{}{eReference1}, eClass.GetEReferences().ToArray())

}

func TestEClassFeaturesGetFromName(t *testing.T) {
	eClass := newEClassExt()
	eAttribute1 := newEAttributeExt()
	eAttribute1.SetName("MyAttribute1")
	eAttribute2 := newEAttributeExt()
	eAttribute2.SetName("MyAttribute2")
	eClass.GetEStructuralFeatures().AddAll(NewImmutableEList([]interface{}{eAttribute1, eAttribute2}))
	assert.Equal(t, eAttribute1, eClass.GetEStructuralFeatureFromString("MyAttribute1"))
	assert.Equal(t, eAttribute2, eClass.GetEStructuralFeatureFromString("MyAttribute2"))
	assert.Equal(t, nil, eClass.GetEStructuralFeatureFromString("MyAttributeUnknown"))
}

func TestEClassAttributeID(t *testing.T) {
	eClass := newEClassExt()
	eAttribute := newEAttributeExt()
	eClass.GetEStructuralFeatures().Add(eAttribute)

	eAttribute.SetID(true)
	assert.Equal(t, eAttribute, eClass.GetEIDAttribute())

	eAttribute.SetID(false)
	assert.Equal(t, nil, eClass.GetEIDAttribute())
}

func TestEClassOperationsGetters(t *testing.T) {
	eClass := newEClassExt()

	// add two operations
	eOperation1 := newEOperationExt()
	eOperation2 := newEOperationExt()
	eOperations := eClass.GetEOperations()
	eOperations.AddAll(NewImmutableEList([]interface{}{eOperation1, eOperation2}))

	// feature ids
	assert.Equal(t, 2, eClass.GetOperationCount())
	assert.Equal(t, eOperation1, eClass.GetEOperation(0))
	assert.Equal(t, eOperation2, eClass.GetEOperation(1))
	assert.Equal(t, nil, eClass.GetEOperation(2))
	assert.Equal(t, 0, eOperation1.GetOperationID())
	assert.Equal(t, 1, eOperation2.GetOperationID())
	assert.Equal(t, 0, eClass.GetOperationID(eOperation1))
	assert.Equal(t, 1, eClass.GetOperationID(eOperation2))

	// collections
	assert.Equal(t, []interface{}{eOperation1, eOperation2}, eClass.GetEAllOperations().ToArray())
	assert.Equal(t, []interface{}{eOperation1, eOperation2}, eClass.GetEOperations().ToArray())

	// insert another one
	eOperation3 := newEOperationExt()
	eOperations.Insert(0, eOperation3)
	assert.Equal(t, []interface{}{eOperation3, eOperation1, eOperation2}, eClass.GetEAllOperations().ToArray())
	assert.Equal(t, []interface{}{eOperation3, eOperation1, eOperation2}, eClass.GetEOperations().ToArray())

	// feature ids
	assert.Equal(t, 3, eClass.GetOperationCount())
	assert.Equal(t, eOperation3, eClass.GetEOperation(0))
	assert.Equal(t, eOperation1, eClass.GetEOperation(1))
	assert.Equal(t, eOperation2, eClass.GetEOperation(2))
	assert.Equal(t, 0, eOperation3.GetOperationID())
	assert.Equal(t, 1, eOperation1.GetOperationID())
	assert.Equal(t, 2, eOperation2.GetOperationID())
}

func TestEClassOperationsGettersWithSuperType(t *testing.T) {
	eClass := newEClassExt()
	eSuperClass := newEClassExt()
	eClass.GetESuperTypes().Add(eSuperClass)

	eOperation1 := newEOperationExt()
	eOperation2 := newEOperationExt()
	eClass.GetEOperations().Add(eOperation1)
	eSuperClass.GetEOperations().Add(eOperation2)

	// collections
	assert.Equal(t, []interface{}{eOperation2}, eSuperClass.GetEAllOperations().ToArray())
	assert.Equal(t, []interface{}{eOperation2}, eSuperClass.GetEOperations().ToArray())

	assert.Equal(t, []interface{}{eOperation2, eOperation1}, eClass.GetEAllOperations().ToArray())
	assert.Equal(t, []interface{}{eOperation1}, eClass.GetEOperations().ToArray())

	// now remove super type
	eClass.GetESuperTypes().Remove(eSuperClass)

	assert.Equal(t, []interface{}{eOperation1}, eClass.GetEAllOperations().ToArray())
	assert.Equal(t, []interface{}{eOperation1}, eClass.GetEOperations().ToArray())
}

func TestEClassAllContainments(t *testing.T) {
	eClass := newEClassExt()
	eSuperClass := newEClassExt()
	eClass.GetESuperTypes().Add(eSuperClass)

	eReference0 := newEReferenceExt()
	eReference1 := newEReferenceExt()
	eReference1.SetContainment(true)
	eReference2 := newEReferenceExt()
	eReference2.SetContainment(true)

	eClass.GetEStructuralFeatures().Add(eReference0)
	eClass.GetEStructuralFeatures().Add(eReference1)
	eSuperClass.GetEStructuralFeatures().Add(eReference2)

	assert.Equal(t, []interface{}{eReference2, eReference1}, eClass.GetEAllContainments().ToArray())

}

func TestEClassContainments(t *testing.T) {
	eClass := newEClassExt()
	// standard ref
	eReference0 := newEReferenceExt()
	// containment ref
	eReference1 := newEReferenceExt()
	eReference1.SetContainment(true)
	// no containment and derived
	eReference2 := newEReferenceExt()

	eClass.GetEStructuralFeatures().AddAll(NewImmutableEList([]interface{}{eReference0, eReference1, eReference2}))

	assert.Equal(t, []interface{}{eReference1}, eClass.GetEContainments().ToArray())
	assert.Equal(t, []interface{}{eReference0, eReference2}, eClass.GetECrossReferences().ToArray())
}

func TestEClassIsSuperTypeOf(t *testing.T) {
	eClass := newEClassExt()
	eOther := newEClassExt()
	eSuperClass := newEClassExt()
	eClass.GetESuperTypes().Add(eSuperClass)

	assert.True(t, eClass.IsSuperTypeOf(eClass))
	assert.True(t, eSuperClass.IsSuperTypeOf(eClass))
	assert.False(t, eClass.IsSuperTypeOf(eSuperClass))
	assert.False(t, eOther.IsSuperTypeOf(eClass))
}

func TestEClassGetOverride(t *testing.T) {
	eClass := newEClassExt()
	eSuperClass := newEClassExt()
	mockOperation1 := &MockEOperation{}
	mockOperation2 := &MockEOperation{}
	mockOperation1.On("SetOperationID", 1)
	mockOperation2.On("SetOperationID", 0)
	eClass.GetESuperTypes().Add(eSuperClass)
	eClass.GetEOperations().Add(mockOperation1)
	eSuperClass.GetEOperations().Add(mockOperation2)

	mockOperation1.On("IsOverrideOf", mockOperation2).Return(true)
	assert.Equal(t, mockOperation1, eClass.GetOverride(mockOperation2))
}
