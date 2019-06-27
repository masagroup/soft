package ecore

import (
	"net/url"
)

// BasicEObject is a basic implementation of an EObject
type BasicEObject struct {
	*Notifier
	internal interface{}
	resource EResource
	container EObject
	containerFeatureID int
	proxyURI *url.URL

}

type EObjectInternal interface {
	EStaticClass() EClass
}

// NewBasicEObject is BasicEObject constructor
func NewBasicEObject() *BasicEObject {
	o := new(BasicEObject)
	o.Notifier = NewNotifier()
	o.internal = o
	o.containerFeatureID = -1
	return o
}

func ( o *BasicEObject) EClass() EClass {
	return o.internal.(EObjectInternal).EStaticClass()
}

func ( o *BasicEObject) EIsProxy() bool {
	return o.proxyURI != nil
}

func ( o *BasicEObject) EResource() EResource {
	return o.resource
}

func ( o *BasicEObject) EContainer() EObject {
	return o.container
}

func ( o *BasicEObject) EContainingFeature() EStructuralFeature {
	if o.container != nil {
		if o.containerFeatureID <= EOPPOSITE_FEATURE_BASE {
			return o.container.EClass().GetEStructuralFeature( EOPPOSITE_FEATURE_BASE - o.containerFeatureID  )
		} else {
			return o.EClass().GetEStructuralFeature( o.containerFeatureID ).(EReference).GetEOpposite()
		}
	}
	return nil
}

func ( o *BasicEObject) EContainmentFeature() EReference {
	return eContainmentFeature( o , o.container, o.containerFeatureID)
}

func eContainmentFeature( o EObject, container EObject, containerFeatureID int ) EReference {
	if ( container != nil ) {
		if( containerFeatureID <= EOPPOSITE_FEATURE_BASE ) {
			feature := container.EClass().GetEStructuralFeature( EOPPOSITE_FEATURE_BASE - containerFeatureID );
			reference , isReference := feature.(EReference)
			if ( isReference ) {
				return reference
			}
        } else {
            feature := o.EClass().GetEStructuralFeature( containerFeatureID );
            reference , isReference := feature.(EReference)
			if ( isReference ) {
				return reference
			}
        }
        panic("The containment feature could not be located")
	}
	return nil
}

func ( o *BasicEObject) EContents() EList {
	return nil
}

func ( o *BasicEObject) EAllContents() EList {
	return nil
}

func ( o *BasicEObject) ECrossReferences() EList {
	return nil
}

func ( o *BasicEObject) EGet(EStructuralFeature) interface{} {
	return nil
}

func ( o *BasicEObject) EGetResolve(EStructuralFeature, bool) interface{} {
	return nil
}

func ( o *BasicEObject) ESet(EStructuralFeature, interface{}) {
}

func ( o *BasicEObject) EIsSet(EStructuralFeature) bool {
	return false
}

func ( o *BasicEObject) EUnset(EStructuralFeature)  {
	
}

func ( o *BasicEObject) EInvoke(EOperation, EList) interface{}  {
	return nil
}

func ( o *BasicEObject) EStaticClass() EClass {
	return GetPackage().GetEObject();
}
