package ecore

import (
	"testing"
)

func TestEcorePackageInstance(t *testing.T) {
	ecorePackage := GetPackage()
	check(t, ecorePackage.GetNsURI(), "http://www.eclipse.org/emf/2002/Ecore", "package NSURI ")
	check(t, ecorePackage.GetNsPrefix(), "ecore", "package NSPrefix ")
}
