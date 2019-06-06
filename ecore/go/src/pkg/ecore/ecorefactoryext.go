package ecore

import (
	"fmt"
	"strconv"
	"time"
)

type EcoreFactoryExt struct {
}

func NewEcoreFactoryExt() *EcoreFactoryExt {
	return &EcoreFactoryExt{}
}

func (factory *EcoreFactoryExt) CreateEBooleanFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseBool(literalValue)
	return value
}

func (factory *EcoreFactoryExt) ConvertEBooleanToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%t", instanceValue)
}

func (factory *EcoreFactoryExt) CreateECharFromString(dataType EDataType, literalValue string) interface{} {
	return literalValue[0]
}

func (factory *EcoreFactoryExt) ConvertECharToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%c", instanceValue)
}

func (factory *EcoreFactoryExt) CreateEDateFromString(dataType EDataType, literalValue string) interface{} {
	strTime, _ := strconv.ParseInt(literalValue, 10, 32)
	return time.Unix(strTime, 0)
}

func (factory *EcoreFactoryExt) ConvertEDateToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue.(time.Time).Unix())
}

func (factory *EcoreFactoryExt) CreateEDoubleFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseFloat(literalValue, 64)
	return value
}

func (factory *EcoreFactoryExt) ConvertEDoubleToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%f", instanceValue)
}

func (factory *EcoreFactoryExt) CreateEFloatFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseFloat(literalValue, 32)
	return float32(value)
}

func (factory *EcoreFactoryExt) ConvertEFloatToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%f", instanceValue)
}

func (factory *EcoreFactoryExt) CreateEIntFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 32)
	return int32(value)
}

func (factory *EcoreFactoryExt) ConvertEIntToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *EcoreFactoryExt) CreateELongFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 64)
	return value
}

func (factory *EcoreFactoryExt) ConvertELongToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *EcoreFactoryExt) CreateEShortFromString(dataType EDataType, literalValue string) interface{} {
	value, _ := strconv.ParseInt(literalValue, 10, 16)
	return int16(value)
}

func (factory *EcoreFactoryExt) ConvertEShortToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%d", instanceValue)
}

func (factory *EcoreFactoryExt) CreateEStringFromString(dataType EDataType, literalValue string) interface{} {
	return literalValue
}

func (factory *EcoreFactoryExt) ConvertEStringToString(dataType EDataType, instanceValue interface{}) string {
	return fmt.Sprintf("%s", instanceValue)
}
