package ecore

import "testing"

func TestFactoryBoolean(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertEBooleanToString(nil, factory.createEBooleanFromString(nil, "true")), "true", "Factory boolean")
	check(t, factory.convertEBooleanToString(nil, factory.createEBooleanFromString(nil, "false")), "false", "Factory boolean")
}

func TestFactoryChar(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertECharToString(nil, factory.createECharFromString(nil, "e")), "e", "Factory char")
}

func TestFactoryDate(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertEDateToString(nil, factory.createEDateFromString(nil, "1559826929")), "1559826929", "Factory date")
}

func TestFactoryDouble(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertEDoubleToString(nil, factory.createEDoubleFromString(nil, "4.987453")), "4.987453", "Factory double")
}

func TestFactoryFloat(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertEFloatToString(nil, factory.createEFloatFromString(nil, "4.987453")), "4.987453", "Factory float")
}

func TestFactoryInt(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertEIntToString(nil, factory.createEIntFromString(nil, "50000000")), "50000000", "Factory int")
}

func TestFactoryLong(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertELongToString(nil, factory.createELongFromString(nil, "5000000000000")), "5000000000000", "Factory long")
}

func TestFactoryShort(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertEShortToString(nil, factory.createEShortFromString(nil, "5000")), "5000", "Factory short")
}

func TestFactoryString(t *testing.T) {
	factory := NewEcoreFactoryExt()
	check(t, factory.convertEStringToString(nil, factory.createEStringFromString(nil, "Hi I'm a string")), "Hi I'm a string", "Factory string")
}
