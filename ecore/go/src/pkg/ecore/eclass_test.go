package ecore

import (
	"testing"
)

func TestEClassInstance(t *testing.T) {
	var eClass EClass = newEClassImpl()
	eClass.SetName("eClass")
}
