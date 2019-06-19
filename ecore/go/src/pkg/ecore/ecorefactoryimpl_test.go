package ecore

import "testing"

func TestFactory(t *testing.T) {
	factory := NewEcoreFactoryExt()
	eDataType := factory.CreateEDataTypeFromContainerAndClassID(nil, EINT)
	checkNot(t, eDataType, nil, "Factory create EDataType")
	check(t, factory.CreateFromString(eDataType, "9"), int32(9), "Factory create variable from string")
}
