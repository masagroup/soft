package ecore

import "testing"

func TestFactoryCreationFromString(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.CreateEIntFromString(nil, "9"), int32(9), "Factory create variable from string")
}

func TestFactoryCreationEClass(t *testing.T) {
	factory := NewEcoreFactoryExt()
	checkNot(t, factory.CreateEClass(), nil, "Factory create EClass")
}
