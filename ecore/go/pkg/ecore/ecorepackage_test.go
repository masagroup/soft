package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestEcorePackageInstance(t *testing.T) {
	ecorePackage := GetPackage()
	assert.Equal(t, ecorePackage.GetNsURI(), "http://www.eclipse.org/emf/2002/Ecore")
	assert.Equal(t, ecorePackage.GetNsPrefix(), "ecore")
}

func TestEcorePackageClassAndAttribute(t *testing.T) {
	ecorePackage := GetPackage()
	eClass := ecorePackage.GetEClass()
	assert.Equal(t, eClass.GetName(), "EClass")
	eClassAbstract := ecorePackage.GetEClass_Abstract()
	assert.Equal(t, eClassAbstract.GetName(), "abstract")
	assert.Equal(t, eClassAbstract.GetEAttributeType().GetName(), "Boolean")

	eAttributes := eClass.GetEAttributes()
	assert.True( t, eAttributes.Contains(eClassAbstract) )
}

