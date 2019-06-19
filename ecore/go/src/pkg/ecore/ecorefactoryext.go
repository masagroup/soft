package ecore

import (
	"fmt"
	"strconv"
	"time"
)

type EcoreFactoryExt struct {
	EcoreFactoryImpl
}

func NewEcoreFactoryExt() *EcoreFactoryExt {
	return &EcoreFactoryExt{
		EcoreFactoryImpl: *GetEcoreFactoryImpl(),
	}
}

func (factory *EcoreFactoryExt) createEBooleanFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseBool(literalValue)
	return value
}

func (factory *EcoreFactoryExt) convertEBooleanToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%t", instanceValue)
}

func (factory *EcoreFactoryExt) createECharFromString(dataType EDataType, literalValue string) interface{} {
	return literalValue[0]
}

func (factory *EcoreFactoryExt) convertECharToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%c", instanceValue)
}

func (factory *EcoreFactoryExt) createEDateFromString(dataType EDataType, literalValue string) interface{} {
	strTime, _ := strconv.ParseInt(literalValue, 10, 32)
	return time.Unix(strTime, 0)
}

func (factory *EcoreFactoryExt) convertEDateToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue.(time.Time).Unix())
}

func (factory *EcoreFactoryExt) createEDoubleFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseFloat(literalValue, 64)
	return value
}

func (factory *EcoreFactoryExt) convertEDoubleToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%f", instanceValue)
}

func (factory *EcoreFactoryExt) createEFloatFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseFloat(literalValue, 32)
	return float32(value)
}

func (factory *EcoreFactoryExt) convertEFloatToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%f", instanceValue)
}

func (factory *EcoreFactoryExt) createEIntFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 32)
	return int32(value)
}

func (factory *EcoreFactoryExt) convertEIntToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *EcoreFactoryExt) createELongFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 64)
	return value
}

func (factory *EcoreFactoryExt) convertELongToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *EcoreFactoryExt) createEShortFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 16)
	return int16(value)
}

func (factory *EcoreFactoryExt) convertEShortToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *EcoreFactoryExt) createEStringFromString(dataType EDataType, literalValue string) interface{} {
	return literalValue
}

func (factory *EcoreFactoryExt) convertEStringToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%s", instanceValue)
}
