package ecore

func GetEObjectID(eObject EObject) string {
	eClass := eObject.EClass()
	eIDAttribute := eClass.GetEIDAttribute()
	if eIDAttribute == nil || !eObject.EIsSet(eIDAttribute) {
		return ""
	} else {
		return convertToString(eIDAttribute.GetEAttributeType(), eObject.EGet(eIDAttribute))
	}
}

func SetEObjectID(eObject EObject, id string) {
	eClass := eObject.EClass()
	eIDAttribute := eClass.GetEIDAttribute()
	if eIDAttribute == nil {
		panic("The object doesn't have an ID feature.")
	} else if len(id) == 0 {
		eObject.EUnset(eIDAttribute)
	} else {
		eObject.ESet(eIDAttribute, createFromString(eIDAttribute.GetEAttributeType(), id))
	}
}

func convertToString(eDataType EDataType, value interface{}) string {
	eFactory := eDataType.GetEPackage().GetEFactoryInstance()
	return eFactory.ConvertToString(eDataType, value)
}

func createFromString(eDataType EDataType, literal string) interface{} {
	eFactory := eDataType.GetEPackage().GetEFactoryInstance()
	return eFactory.CreateFromString(eDataType, literal)
}
