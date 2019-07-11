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

func TestFactoryBoolean(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertEBooleanToString(nil, factory.createEBooleanFromString(nil, "true")), "true")
	assert.Equal(t, factory.convertEBooleanToString(nil, factory.createEBooleanFromString(nil, "false")), "false")
}

func TestFactoryChar(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertECharToString(nil, factory.createECharFromString(nil, "e")), "e")
}

func TestFactoryDate(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertEDateToString(nil, factory.createEDateFromString(nil, "1559826929")), "1559826929")
}

func TestFactoryDouble(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertEDoubleToString(nil, factory.createEDoubleFromString(nil, "4.987453")), "4.987453")
}

func TestFactoryFloat(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertEFloatToString(nil, factory.createEFloatFromString(nil, "4.987453")), "4.987453")
}

func TestFactoryInt(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertEIntToString(nil, factory.createEIntFromString(nil, "50000000")), "50000000")
}

func TestFactoryLong(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertELongToString(nil, factory.createELongFromString(nil, "5000000000000")), "5000000000000")
}

func TestFactoryShort(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertEShortToString(nil, factory.createEShortFromString(nil, "5000")), "5000")
}

func TestFactoryString(t *testing.T) {
	factory := newEcoreFactoryExt()
	assert.Equal(t, factory.convertEStringToString(nil, factory.createEStringFromString(nil, "Hi I'm a string")), "Hi I'm a string")
}
