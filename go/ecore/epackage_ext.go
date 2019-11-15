// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

type ePackageExtAdapter struct {
	*Adapter
	pack *EPackageExt
}

func (a *ePackageExtAdapter) NotifyChanged(notification ENotification) {
	eventType := notification.GetEventType()
	if eventType != REMOVING_ADAPTER {
		featureID := notification.GetFeatureID()
		if featureID == EPACKAGE__ECLASSIFIERS {
			a.pack.nameToClassifier = nil
		}
	}
}

// EPackageExt is the extension of the model object 'EFactory'
type EPackageExt struct {
	*ePackageImpl
	adapter          EAdapter
	nameToClassifier map[string]EClassifier
}

func NewEPackageExt() *EPackageExt {
	pack := new(EPackageExt)
	pack.ePackageImpl = newEPackageImpl()
	pack.adapter = &ePackageExtAdapter{Adapter: NewAdapter(), pack: pack}
	pack.SetInterfaces(pack)
	pack.EAdapters().Add(pack.adapter)
	return pack
}

func (pack *EPackageExt) GetEClassifier(classifier string) EClassifier {
	if pack.nameToClassifier == nil {
		pack.nameToClassifier = make(map[string]EClassifier)
		for itClassifier := pack.GetEClassifiers().Iterator(); itClassifier.HasNext(); {
			classifier := itClassifier.Next().(EClassifier)
			pack.nameToClassifier[classifier.GetName()] = classifier
		}
	}
	return pack.nameToClassifier[classifier]
}
