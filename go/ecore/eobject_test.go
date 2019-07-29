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

func TestEObjectContents(t *testing.T) {
	ecorePackage := GetPackage()
	eClass := ecorePackage.GetEClass()
	assert.Equal(t, eClass.GetName(), "EClass")
	eClassAbstract := ecorePackage.GetEClass_Abstract()
	assert.Equal(t, eClassAbstract.GetName(), "abstract")
	assert.Equal(t, eClassAbstract.GetEAttributeType().GetName(), "EBoolean")

	eFeatures := eClass.GetEStructuralFeatures()
	assert.True(t, eFeatures.Contains(eClassAbstract))
}
