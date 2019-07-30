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

func TestImmutableEListIterate(t *testing.T) {
	var iDatas []interface{}
	iDatas = append(iDatas, 0, 2, 4, 6)
	arr := NewImmutableEList(iDatas)
	i := 0
	for it := arr.Iterator(); it.HasNext(); {
		assert.Equal(t, it.Next(), i)
		i += 2
	}
	assert.Equal(t, 8, i)
}
