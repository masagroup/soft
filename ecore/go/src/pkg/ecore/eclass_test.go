package ecore

import (
	"testing"
)

func TestEClassInstance(t *testing.T) {
	var eClass EClass = &eClassImpl{}
	eClass.SetName("eClass")
}
