// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// ENotificationChain is an accumulator of notifications.
// As notifications are produced,they are accumulated in a chain,
// and possibly even merged, before finally being dispatched to the notifier.
type ENotificationChain interface {

	// Add Adds a notification to the chain.
	Add(ENotification) bool

	// Dispatch Dispatches each notification to the appropriate notifier via notifier.ENotify method
	Dispatch()
}
