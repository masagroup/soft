// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

type ENotifier interface {
	EAdapters() EList

	EDeliver() bool

	ESetDeliver(bool)

	ENotify(ENotification)

	ENotificationRequired() bool
}
