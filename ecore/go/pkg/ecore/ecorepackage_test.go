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
