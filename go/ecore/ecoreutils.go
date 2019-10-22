package ecore

import (
	"net/url"
	"strings"
)

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

func GetURI(eObject EObject) *url.URL {
	if eObject.EIsProxy() {
		return eObject.(EObjectInternal).EProxyURI()
	} else {
		resource := eObject.EResource()
		if resource != nil {
			uri := resource.GetURI()
			uri.Fragment = resource.GetURIFragment(eObject)
			return uri
		} else {
			id := GetEObjectID(eObject)
			if len(id) == 0 {
				return &url.URL{Fragment: "//" + getRelativeURIFragmentPath(nil, eObject, false)}
			} else {
				return &url.URL{Fragment: id}
			}
		}
	}
}

func getRelativeURIFragmentPath(ancestor EObject, descendant EObject, resolve bool) string {
	if ancestor == descendant {
		return ""
	}
	eObject := descendant
	eContainer := eObject.EContainer()
	visited := make(map[EObject]bool)
	fragmentPath := []string{}
	for {
		if eContainer == nil {
			break
		}
		if _, v := visited[eObject]; v {
			break
		}
		fragmentPath = append([]string{eContainer.(EObjectInternal).EURIFragmentSegment(eObject.EContainingFeature(), eObject)}, fragmentPath...)
		eObject = eContainer
		if eContainer == ancestor {
			break
		}
		eContainer = eObject.EContainer()
	}
	if eObject != ancestor && ancestor != nil {
		panic("The ancestor not found")
	}

	return strings.Join(fragmentPath, "/")
}
