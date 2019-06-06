package ecore

import "testing"

func TestFactoryBoolean(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertEBooleanToString(nil, factory.CreateEBooleanFromString(nil, "true")), "true", "Factory boolean")
	check(t, factory.ConvertEBooleanToString(nil, factory.CreateEBooleanFromString(nil, "false")), "false", "Factory boolean")
}

func TestFactoryChar(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertECharToString(nil, factory.CreateECharFromString(nil, "e")), "e", "Factory char")
}

func TestFactoryDate(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertEDateToString(nil, factory.CreateEDateFromString(nil, "1559826929")), "1559826929", "Factory date")
}

func TestFactoryDouble(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertEDoubleToString(nil, factory.CreateEDoubleFromString(nil, "4.987453")), "4.987453", "Factory double")
}

func TestFactoryFloat(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertEFloatToString(nil, factory.CreateEFloatFromString(nil, "4.987453")), "4.987453", "Factory float")
}

func TestFactoryInt(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertEIntToString(nil, factory.CreateEIntFromString(nil, "50000000")), "50000000", "Factory int")
}

func TestFactoryLong(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertELongToString(nil, factory.CreateELongFromString(nil, "5000000000000")), "5000000000000", "Factory long")
}

func TestFactoryShort(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertEShortToString(nil, factory.CreateEShortFromString(nil, "5000")), "5000", "Factory short")
}

func TestFactoryString(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.ConvertEStringToString(nil, factory.CreateEStringFromString(nil, "Hi I'm a string")), "Hi I'm a string", "Factory string")
}
