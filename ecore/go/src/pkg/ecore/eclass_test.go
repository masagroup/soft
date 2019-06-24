package ecore

import (
	"testing"
)

func TestEClassInstance(t *testing.T) {
	eClass := newEClassImpl()
	eClass.SetName("eClass")
}
