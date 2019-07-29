package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestECoreEClasses(t *testing.T) {
	f :=  GetFactory()
	p := GetPackage()
	assert.Equal(t, p.GetEClass() , f.CreateEClass().EClass() )
	assert.Equal(t, p.GetEAttribute() , f.CreateEAttribute().EClass() )
	assert.Equal(t, p.GetEOperation() , f.CreateEOperation().EClass() )
	assert.Equal(t, p.GetEDataType() , f.CreateEDataType().EClass() )
}
