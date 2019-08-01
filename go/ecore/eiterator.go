// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// EIterator is an interator
type EIterator interface {
	HasNext() bool
	Next() interface{}
}
