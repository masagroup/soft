// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

// EList is the interface for dynamic containers
type EList interface {
	Add(interface{}) bool

	AddAll(EList) bool

	Insert(int, interface{}) bool

	InsertAll(int, EList) bool

	MoveObject(int, interface{})

	Move(int, int) interface{}

	Get(int) interface{}

	Set(int, interface{})

	RemoveAt(int) interface{}

	Remove(interface{}) bool

	Size() int

	Clear()

	Empty() bool

	Contains(interface{}) bool

	IndexOf(interface{}) int

	Iterator() EIterator

	ToArray() []interface{}
}
