package ecore

import (
	"net/url"
	"strconv"
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
	EGetFromID(featureID int , resolve bool , core bool ) interface{}
	ESetFromID( featureID int, newValue interface{})
	EUnsetFromID(featureID int )
	EIsSetFromID(featureID int ) bool
	EInvokeFromID(operationID int, arguments EList) interface{}
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
	if ( o.resource == nil) {
		if ( o.container != nil ) {
			o.resource = o.container.EResource()
		}
	}
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

func ( o *BasicEObject) eDerivedStructuralFeatureID( eFeature EStructuralFeature) int {
	if !o.EClass().GetEAllStructuralFeatures().Contains( eFeature ) {
		panic( "The feature '"+ eFeature.GetName() + "' is not a valid feature" )
	} 
    return eFeature.GetFeatureID()
}

func ( o *BasicEObject) eDerivedOperationID( eOperation EOperation ) int {
	if !o.EClass().GetEAllOperations().Contains( eOperation ) {
		panic( "The operation '"+ eOperation.GetName() + "' is not a valid operation" )
	} 
    return eOperation.GetOperationID()
}

func ( o *BasicEObject) EGet(eFeature EStructuralFeature) interface{} {
	return o.eGetFromFeature( eFeature, true, true )
}

func ( o *BasicEObject) EGetResolve( eFeature EStructuralFeature, resolve bool) interface{} {
	return o.eGetFromFeature( eFeature, resolve, true )
}

func ( o *BasicEObject) eGetFromFeature(eFeature EStructuralFeature , resolve bool , core bool ) interface{} {
	featureID := o.eDerivedStructuralFeatureID( eFeature )
    if featureID >= 0 {
		return o.internal.(EObjectInternal).EGetFromID( featureID, resolve, core )
	}
    panic("The feature '" + eFeature.GetName() + "' is not a valid feature")
}

func ( o *BasicEObject) EGetFromID(featureID int , resolve bool , core bool ) interface{} {
	eFeature := o.EClass().GetEStructuralFeature( featureID );
	if eFeature == nil {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
	return nil
}

func ( o *BasicEObject) ESet( eFeature EStructuralFeature, newValue interface{}) {
	featureID := o.eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 ) {
		o.internal.(EObjectInternal).ESetFromID( featureID, newValue );
	} else {
		panic("The feature '" + eFeature.GetName() + "' is not a valid feature")
	}
}

func ( o *BasicEObject) ESetFromID( featureID int, newValue interface{}) {
	eFeature := o.EClass().GetEStructuralFeature( featureID );
	if ( eFeature == nil ) {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
}

func ( o *BasicEObject) EIsSet( eFeature EStructuralFeature) bool {
	featureID := o.eDerivedStructuralFeatureID( eFeature )
    if( featureID >= 0 ) {
		return o.internal.(EObjectInternal).EIsSetFromID( featureID )
	}
	panic( "The feature '" + eFeature.GetName() + "' is not a valid feature" )
}

func ( o *BasicEObject) EIsSetFromID(featureID int ) bool {
	eFeature := o.EClass().GetEStructuralFeature( featureID )
	if eFeature == nil {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
    return false;
}

func ( o *BasicEObject) EUnset(eFeature EStructuralFeature)  {
	featureID := o.eDerivedStructuralFeatureID( eFeature )
    if( featureID >= 0 ) {
		o.internal.(EObjectInternal).EUnsetFromID( featureID  )
	} else {
		panic("The feature '" + eFeature.GetName() + "' is not a valid feature")
	}
}

func ( o *BasicEObject) EUnsetFromID(featureID int ) {
	eFeature := o.EClass().GetEStructuralFeature( featureID )
	if eFeature == nil {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
}

func ( o *BasicEObject) EInvoke(eOperation EOperation, arguments EList) interface{}  {
	operationID := o.eDerivedOperationID( eOperation );
    if( operationID >= 0 ) {
		return o.internal.(EObjectInternal).EInvokeFromID( operationID, arguments )
	}
    panic("The operation '" + eOperation.GetName() + "' is not a valid operation");
}

func ( o *BasicEObject) EInvokeFromID(operationID int, arguments EList) interface{}  {
	eOperation := o.EClass().GetEOperation( operationID )
	if eOperation == nil {
		panic( "Invalid operationID: " + strconv.Itoa(operationID) )
	}
	return nil
}

func ( o *BasicEObject) EStaticClass() EClass {
	return GetPackage().GetEObject();
}
