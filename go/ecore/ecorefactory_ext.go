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
	"fmt"
	"strconv"
	"time"
)

type ecoreFactoryExt struct {
	*ecoreFactoryImpl
}

func newEcoreFactoryExt() *ecoreFactoryExt {
	factory := new(ecoreFactoryExt)
	factory.ecoreFactoryImpl = newEcoreFactoryImpl()
	factory.interfaces = factory
	return factory
}

func (factory *ecoreFactoryExt) createEBooleanFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseBool(literalValue)
	return value
}

func (factory *ecoreFactoryExt) convertEBooleanToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%t", instanceValue)
}

func (factory *ecoreFactoryExt) createECharFromString(dataType EDataType, literalValue string) interface{} {
	return literalValue[0]
}

func (factory *ecoreFactoryExt) convertECharToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%c", instanceValue)
}

func (factory *ecoreFactoryExt) createEDateFromString(dataType EDataType, literalValue string) interface{} {
	strTime, _ := strconv.ParseInt(literalValue, 10, 32)
	return time.Unix(strTime, 0)
}

func (factory *ecoreFactoryExt) convertEDateToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue.(time.Time).Unix())
}

func (factory *ecoreFactoryExt) createEDoubleFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseFloat(literalValue, 64)
	return value
}

func (factory *ecoreFactoryExt) convertEDoubleToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%f", instanceValue)
}

func (factory *ecoreFactoryExt) createEFloatFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseFloat(literalValue, 32)
	return float32(value)
}

func (factory *ecoreFactoryExt) convertEFloatToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%f", instanceValue)
}

func (factory *ecoreFactoryExt) createEIntFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.Atoi(literalValue)
	return int(value)
}

func (factory *ecoreFactoryExt) convertEIntToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *ecoreFactoryExt) createELongFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 64)
	return int(value)
}

func (factory *ecoreFactoryExt) convertELongToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *ecoreFactoryExt) createEShortFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 16)
	return int(value)
}

func (factory *ecoreFactoryExt) convertEShortToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *ecoreFactoryExt) createEStringFromString(dataType EDataType, literalValue string) interface{} {
	return literalValue
}

func (factory *ecoreFactoryExt) convertEStringToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%s", instanceValue)
}
