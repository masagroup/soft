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
	"net/url"
	"strconv"
)

// BasicEObject is a basic implementation of an EObject
type BasicEObject struct {
	*Notifier
	interfaces interface{}
	resource EResource
	container EObject
	containerFeatureID int
	proxyURI *url.URL

}

type EObjectInternal interface {
	EObject

	EStaticClass() EClass

	ESetResource( resource EResource, notifications ENotificationChain ) ENotificationChain
	EInverseAdd( otherEnd EObject, featureID int, notifications ENotificationChain ) ENotificationChain
	EInverseRemove( otherEnd EObject, featureID int, notifications ENotificationChain ) ENotificationChain
	
	EGetFromID(featureID int , resolve bool , core bool ) interface{}
	ESetFromID( featureID int, newValue interface{})
	EUnsetFromID(featureID int )
	EIsSetFromID(featureID int ) bool
	EInvokeFromID(operationID int, arguments EList) interface{}

	EBasicInverseAdd( otherEnd EObject, featureID int, notifications ENotificationChain ) ENotificationChain
	EBasicInverseRemove( otherEnd EObject, featureID int, notifications ENotificationChain ) ENotificationChain
	
	EProxyUri() *url.URL
	ESetProxyURI( uri *url.URL)
	EResolveProxy( proxy EObject) EObject
}

// NewBasicEObject is BasicEObject constructor
func NewBasicEObject() *BasicEObject {
	o := new(BasicEObject)
	o.Notifier = NewNotifier()
	o.interfaces = o
	o.containerFeatureID = -1
	return o
}

func ( o *BasicEObject) EClass() EClass {
	return o.interfaces.(EObjectInternal).EStaticClass()
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

func ( o *BasicEObject) eContainerFeatureID() int {
	return o.containerFeatureID
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

func ( o *BasicEObject) eDerivedStructuralFeatureID( feature EStructuralFeature) int {
	if !o.EClass().GetEAllStructuralFeatures().Contains( feature ) {
		panic( "The feature '"+ feature.GetName() + "' is not a valid feature" )
	} 
    return feature.GetFeatureID()
}

func ( o *BasicEObject) eDerivedOperationID( operation EOperation ) int {
	if !o.EClass().GetEAllOperations().Contains( operation ) {
		panic( "The operation '"+ operation.GetName() + "' is not a valid operation" )
	} 
    return operation.GetOperationID()
}

func ( o *BasicEObject) EGet(feature EStructuralFeature) interface{} {
	return o.eGetFromFeature( feature, true, true )
}

func ( o *BasicEObject) EGetResolve( feature EStructuralFeature, resolve bool) interface{} {
	return o.eGetFromFeature( feature, resolve, true )
}

func ( o *BasicEObject) eGetFromFeature(feature EStructuralFeature , resolve bool , core bool ) interface{} {
	featureID := o.eDerivedStructuralFeatureID( feature )
    if featureID >= 0 {
		return o.interfaces.(EObjectInternal).EGetFromID( featureID, resolve, core )
	}
    panic("The feature '" + feature.GetName() + "' is not a valid feature")
}

func ( o *BasicEObject) EGetFromID(featureID int , resolve bool , core bool ) interface{} {
	feature := o.EClass().GetEStructuralFeature( featureID );
	if feature == nil {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
	return nil
}

func ( o *BasicEObject) ESet( feature EStructuralFeature, newValue interface{}) {
	featureID := o.eDerivedStructuralFeatureID( feature );
    if( featureID >= 0 ) {
		o.interfaces.(EObjectInternal).ESetFromID( featureID, newValue );
	} else {
		panic("The feature '" + feature.GetName() + "' is not a valid feature")
	}
}

func ( o *BasicEObject) ESetFromID( featureID int, newValue interface{}) {
	feature := o.EClass().GetEStructuralFeature( featureID );
	if ( feature == nil ) {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
}

func ( o *BasicEObject) EIsSet( feature EStructuralFeature) bool {
	featureID := o.eDerivedStructuralFeatureID( feature )
    if( featureID >= 0 ) {
		return o.interfaces.(EObjectInternal).EIsSetFromID( featureID )
	}
	panic( "The feature '" + feature.GetName() + "' is not a valid feature" )
}

func ( o *BasicEObject) EIsSetFromID(featureID int ) bool {
	feature := o.EClass().GetEStructuralFeature( featureID )
	if feature == nil {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
    return false;
}

func ( o *BasicEObject) EUnset(feature EStructuralFeature)  {
	featureID := o.eDerivedStructuralFeatureID( feature )
    if( featureID >= 0 ) {
		o.interfaces.(EObjectInternal).EUnsetFromID( featureID  )
	} else {
		panic("The feature '" + feature.GetName() + "' is not a valid feature")
	}
}

func ( o *BasicEObject) EUnsetFromID(featureID int ) {
	feature := o.EClass().GetEStructuralFeature( featureID )
	if feature == nil {
		panic( "Invalid featureID: " + strconv.Itoa(featureID) )
	}
}

func ( o *BasicEObject) EInvoke(operation EOperation, arguments EList) interface{}  {
	operationID := o.eDerivedOperationID( operation );
    if( operationID >= 0 ) {
		return o.interfaces.(EObjectInternal).EInvokeFromID( operationID, arguments )
	}
    panic("The operation '" + operation.GetName() + "' is not a valid operation");
}

func ( o *BasicEObject) EInvokeFromID(operationID int, arguments EList) interface{}  {
	operation := o.EClass().GetEOperation( operationID )
	if operation == nil {
		panic( "Invalid operationID: " + strconv.Itoa(operationID) )
	}
	return nil
}

func ( o *BasicEObject) EStaticClass() EClass {
	return GetPackage().GetEObject();
}


func ( o *BasicEObject) ESetResource( resource EResource , n ENotificationChain ) ENotificationChain {
	return n
}

func ( o *BasicEObject) EInverseAdd( otherEnd EObject, featureID int, n ENotificationChain ) ENotificationChain {
	notifications := n;
    if( featureID >= 0 ) {
        return o.interfaces.(EObjectInternal).EBasicInverseAdd( otherEnd, featureID, notifications )
	} else {
        notifications = o.EBasicRemoveFromContainer( notifications );
        return o.EBasicSetContainer( otherEnd, featureID, notifications )
	}
	return notifications
}

func ( o *BasicEObject) EInverseRemove( otherEnd EObject, featureID int, n ENotificationChain ) ENotificationChain {
	if( featureID >= 0 ) {
		return o.interfaces.(EObjectInternal).EBasicInverseRemove( otherEnd, featureID, n )
	} else {
		return o.EBasicSetContainer( nil, featureID, n )
	}
}

func ( o *BasicEObject) EProxyUri() *url.URL {
	return o.proxyURI
}

func ( o *BasicEObject) ESetProxyURI( uri *url.URL) {
	o.proxyURI = uri
}

func ( o *BasicEObject) EResolveProxy( proxy EObject) EObject {
	return nil
}


func ( o *BasicEObject) EBasicInverseAdd( otherEnd EObject, featureID int, notifications ENotificationChain ) ENotificationChain {
	return nil
}

func ( o *BasicEObject) EBasicInverseRemove( otherEnd EObject, featureID int, notifications ENotificationChain ) ENotificationChain {
	return nil
}

func ( o *BasicEObject) EBasicSetContainer( newContainer EObject, newContainerFeatureID int , n ENotificationChain ) ENotificationChain {
    notifications := n;
	oldResource := o.resource
	oldContainer := o.container
    oldContainerFeatureID := o.containerFeatureID;

    // resource
    var newResource EResource
    if( oldResource != nil ) {
        if( newContainer != nil  && eContainmentFeature( o, newContainer, newContainerFeatureID ) == nil ) {
            //auto list = std::dynamic_pointer_cast<ENotifyingList<std::shared_ptr<EObject>>>( oldResource->getContents() );
            //notifications = list->remove( thisPtr, notifications );

            o.resource = nil
            newResource = newContainer.EResource()
        } else {
			oldResource = nil
		}
    } else {
        if( oldContainer != nil) {
			oldResource = oldContainer.EResource()
		}	
        
        if( newContainer != nil) {
			newResource = newContainer.EResource()
		}
    }

    if( oldResource != nil && oldResource != newResource ) {
		oldResource.Detached( o )
	}
    
    if( newResource != nil && newResource != oldResource ) {
		newResource.Attached( o )
	}
 
    // basic set
    o.container = newContainer
    o.containerFeatureID = newContainerFeatureID
	o.resource = newResource

    // notification
    if( o.ENotificationRequired() ) {
        if( oldContainer != nil  && oldContainerFeatureID >= 0 && oldContainerFeatureID != newContainerFeatureID ) {
			notification := NewNotificationByFeatureID( o, SET, oldContainerFeatureID, oldContainer, nil, -1 )
            if( notifications != nil ) {
                notifications.Add( notification );
			} else {
				notifications = notification;
			}
        }
        if( newContainerFeatureID >= 0 ) {
			var c EObject 
			if oldContainerFeatureID == newContainerFeatureID {
				c = oldContainer
			} 
			notification := NewNotificationByFeatureID( o, SET, newContainerFeatureID, c , newContainer, -1 )
            if( notifications != nil ) {
                notifications.Add( notification )
			} else {
				notifications = notification
			}
        }
    }
    return notifications
}

func ( o *BasicEObject) EBasicRemoveFromContainer( notifications ENotificationChain ) ENotificationChain {
    if( o.containerFeatureID >= 0 ) {
		return o.EBasicRemoveFromContainerFeature( notifications )
	} else {
        if( o.container != nil ) {
			return o.container.(EObjectInternal).EInverseRemove( o, EOPPOSITE_FEATURE_BASE - o.containerFeatureID, notifications )
		}
    }
    return notifications
}

func ( o *BasicEObject) EBasicRemoveFromContainerFeature( notifications ENotificationChain ) ENotificationChain {
    reference , isReference := o.EClass().GetEStructuralFeature( o.containerFeatureID ).(EReference);
    if( isReference ) {
        inverseFeature := reference.GetEOpposite()
        if( o.container != nil && inverseFeature != nil ) {
			return o.container.(EObjectInternal).EInverseRemove( o, inverseFeature.GetFeatureID(), notifications )
		}
    }
    return notifications;
}
