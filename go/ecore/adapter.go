// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// Adapter is basic implementation of EAdapter interface
type Adapter struct {
	target ENotifier
}

// NewAdapter Constructor
func NewAdapter() *Adapter {
	return &Adapter{target: nil}
}

// GetTarget Returns the target from which the adapter receives notification.
func (adapter *Adapter) GetTarget() ENotifier {
	return adapter.target
}

// SetTarget Sets the target from which the adapter will receive notification.
func (adapter *Adapter) SetTarget(notifier ENotifier) {
	adapter.target = notifier
}
