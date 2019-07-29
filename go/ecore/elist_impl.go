// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

import "strconv"

type abstractEList interface {
	doAdd(elem interface{})

	doAddAll(list EList) bool

	doInsert(index int, elem interface{})

	doInsertAll(index int, list EList) bool

	doSet(index int, elem interface{}) interface{}

	didAdd(index int, elem interface{})

	didSet(index int, newElem interface{}, oldElem interface{})

	didRemove(index int, old interface{})

	didClear(oldObjects []interface{})

	didMove(newIndex int, movedObject interface{}, oldIndex int)

	didChange()
}

// arrayEList is an array of a dynamic size
type arrayEList struct {
	interfaces interface{}
	data       []interface{}
	isUnique   bool
}

type immutableEList struct {
	data []interface{}
}

// NewEmptyArrayEList return a new ArrayEList
func NewEmptyArrayEList() *arrayEList {
	a := new(arrayEList)
	a.interfaces = a
	a.data = []interface{}{}
	a.isUnique = false
	return a
}

// NewArrayEList return a new ArrayEList
func NewArrayEList(data []interface{}) *arrayEList {
	a := new(arrayEList)
	a.interfaces = a
	a.data = data
	a.isUnique = false
	return a
}

// NewUniqueArrayEList return a new ArrayEList with isUnique set as true
func NewUniqueArrayEList(data []interface{}) *arrayEList {
	a := new(arrayEList)
	a.interfaces = a
	a.data = data
	a.isUnique = true
	return a
}

// NewImmutableEList return a new ImmutableEList
func NewImmutableEList(data []interface{}) *immutableEList {
	return &immutableEList{data: data}
}

type listIterator struct {
	cursor int
	list   EList
}

// Next return the current value of the iterator
func (it *listIterator) Next() interface{} {
	i := it.cursor
	if i >= it.list.Size() {
		panic("Not such an element")
	}
	it.cursor = i + 1
	return it.list.Get(i)
}

// HasNext make the iterator go further in the array
func (it *listIterator) HasNext() bool {
	return it.cursor < it.list.Size()
}

// Remove all elements in list that already are in arr.data
func (arr *arrayEList) removeDuplicated(list EList) *arrayEList {
	newArr := NewArrayEList([]interface{}{})
	for it := list.Iterator(); it.HasNext(); {
		value := it.Next()
		if !newArr.Contains(value) && !arr.Contains(value) {
			newArr.Add(value)
		}
	}
	return newArr
}

func (arr *arrayEList) Add(elem interface{}) bool {
	if arr.isUnique && arr.Contains(elem) {
		return false
	}
	arr.interfaces.(abstractEList).doAdd(elem)
	return true
}

// Add a new element to the array
func (arr *arrayEList) doAdd(e interface{}) {
	size := len(arr.data)
	arr.data = append(arr.data, e)
	// events
	interfaces := arr.interfaces.(abstractEList)
	interfaces.didAdd(size, e)
	interfaces.didChange()
}

// AddAll elements of an array in the current one
func (arr *arrayEList) AddAll(list EList) bool {
	if arr.isUnique {
		list = arr.removeDuplicated(list)
		if list.Size() == 0 {
			return false
		}
	}
	arr.interfaces.(abstractEList).doAddAll(list)
	return true
}

func (arr *arrayEList) doAddAll(list EList) bool {
	data := list.ToArray()
	arr.data = append(arr.data, data...)
	interfaces := arr.interfaces.(abstractEList)
	// events
	for i, element := range data {
		interfaces.didAdd(i, element)
		interfaces.didChange()
	}
	return len(data) != 0
}

// Insert an element in the array
func (arr *arrayEList) Insert(index int, elem interface{}) bool {
	if index < 0 || index > arr.Size() {
		panic("Index out of bounds: index=" + strconv.Itoa(index) + " size=" + strconv.Itoa(arr.Size()))
	}
	if arr.isUnique && arr.Contains(elem) {
		return false
	}
	arr.interfaces.(abstractEList).doInsert(index, elem)
	return true
}

func (arr *arrayEList) doInsert(index int, e interface{}) {
	arr.data = append(arr.data, nil)
	copy(arr.data[index+1:], arr.data[index:])
	arr.data[index] = e
	// events
	interfaces := arr.interfaces.(abstractEList)
	interfaces.didAdd(index, e)
	interfaces.didChange()
}

// InsertAll element of an array at a given position
func (arr *arrayEList) InsertAll(index int, list EList) bool {
	if index < 0 || index > arr.Size() {
		panic("Index out of bounds: index=" + strconv.Itoa(index) + " size=" + strconv.Itoa(arr.Size()))
	}
	if arr.isUnique {
		list = arr.removeDuplicated(list)
		if list.Size() == 0 {
			return false
		}
	}
	arr.interfaces.(abstractEList).doInsertAll(index, list)
	return true
}

func (arr *arrayEList) doInsertAll(index int, list EList) bool {
	data := list.ToArray()
	arr.data = append(arr.data[:index], append(data, arr.data[index:]...)...)
	// events
	interfaces := arr.interfaces.(abstractEList)
	for i, element := range data {
		interfaces.didAdd(i+index, element)
		interfaces.didChange()
	}
	return len(data) != 0
}

// Move an element to the given index
func (arr *arrayEList) MoveObject(newIndex int, elem interface{}) {
	oldIndex := arr.IndexOf(elem)
	if oldIndex == -1 {
		panic("Index out of bounds")
	}
	arr.Move(oldIndex, newIndex)
}

// Swap move an element from oldIndex to newIndex
func (arr *arrayEList) Move(oldIndex, newIndex int) interface{} {
	if oldIndex < 0 || oldIndex >= arr.Size() ||
		newIndex < 0 || newIndex > arr.Size() {
		panic("Index out of bounds: oldIndex=" + strconv.Itoa(oldIndex) + " newIndex=" + strconv.Itoa(newIndex) + " size=" + strconv.Itoa(arr.Size()))
	}
	object := arr.data[oldIndex]
	copy(arr.data[oldIndex:], arr.data[oldIndex+1:])
	if newIndex > oldIndex {
		newIndex--
	}
	copy(arr.data[newIndex+1:], arr.data[newIndex:])
	arr.data[newIndex] = object
	// events
	interfaces := arr.interfaces.(abstractEList)
	interfaces.didMove(newIndex, object, oldIndex)
	interfaces.didChange()
	return object
}

// RemoveAt remove an element at a given position
func (arr *arrayEList) RemoveAt(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds: index=" + strconv.Itoa(index) + " size=" + strconv.Itoa(arr.Size()))
	}
	object := arr.Get(index)
	arr.data = append(arr.data[:index], arr.data[index+1:]...)
	// events
	interfaces := arr.interfaces.(abstractEList)
	interfaces.didRemove(index, object)
	interfaces.didChange()
	return object
}

// Remove an element in an array
func (arr *arrayEList) Remove(elem interface{}) bool {
	index := arr.IndexOf(elem)
	if index == -1 {
		return false
	}
	arr.interfaces.(EList).RemoveAt(index)
	return true
}

// Get an element of the array
func (arr *arrayEList) Get(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds: index=" + strconv.Itoa(index) + " size=" + strconv.Itoa(arr.Size()))
	}
	return arr.data[index]
}

func (arr *arrayEList) doSet(index int, elem interface{}) interface{} {
	old := arr.data[index]
	arr.data[index] = elem
	// events
	interfaces := arr.interfaces.(abstractEList)
	interfaces.didSet(index, elem, old)
	interfaces.didChange()
	return old
}

// Set an element of the array
func (arr *arrayEList) Set(index int, elem interface{}) {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds: index=" + strconv.Itoa(index) + " size=" + strconv.Itoa(arr.Size()))
	}
	if !arr.Contains(elem) {
		arr.interfaces.(abstractEList).doSet(index, elem)
	}
}

// Size count the number of element in the array
func (arr *arrayEList) Size() int {
	return len(arr.data)
}

// Clear remove all elements of the array
func (arr *arrayEList) Clear() {
	arr.data = make([]interface{}, 0)
}

// Empty return true if the array contains 0 element
func (arr *arrayEList) Empty() bool {
	return arr.Size() == 0
}

// Contains return if an array contains or not an element
func (arr *arrayEList) Contains(elem interface{}) bool {
	return arr.IndexOf(elem) != -1
}

// IndexOf return the index on an element in an array, else return -1
func (arr *arrayEList) IndexOf(elem interface{}) int {
	for i, value := range arr.data {
		if value == elem {
			return i
		}
	}
	return -1
}

// Iterator through the array
func (arr *arrayEList) Iterator() EIterator {
	return &listIterator{list: arr}
}

func (arr *arrayEList) ToArray() []interface{} {
	return arr.data
}

func (arr *arrayEList) didAdd(index int, elem interface{}) {

}

func (arr *arrayEList) didSet(index int, newElem interface{}, oldElem interface{}) {

}

func (arr *arrayEList) didRemove(index int, old interface{}) {

}

func (arr *arrayEList) didClear(oldObjects []interface{}) {

}

func (arr *arrayEList) didMove(newIndex int, movedObject interface{}, oldIndex int) {

}

func (arr *arrayEList) didChange() {

}

func (arr *immutableEList) Add(elem interface{}) bool {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) AddAll(list EList) bool {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) Insert(index int, elem interface{}) bool {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) InsertAll(index int, list EList) bool {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) MoveObject(newIndex int, elem interface{}) {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) Move(oldIndex, newIndex int) interface{} {
	panic("Immutable list can't be modified")
}

// Get an element of the array
func (arr *immutableEList) Get(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds: index=" + strconv.Itoa(index) + " size=" + strconv.Itoa(arr.Size()))
	}
	return arr.data[index]
}

func (arr *immutableEList) Set(index int, elem interface{}) {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) RemoveAt(index int) interface{} {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) Remove(elem interface{}) bool {
	panic("Immutable list can't be modified")
}

// Size count the number of element in the array
func (arr *immutableEList) Size() int {
	return len(arr.data)
}

func (arr *immutableEList) Clear() {
	panic("Immutable list can't be modified")
}

// Empty return true if the array contains 0 element
func (arr *immutableEList) Empty() bool {
	return arr.Size() == 0
}

// Contains return if an array contains or not an element
func (arr *immutableEList) Contains(elem interface{}) bool {
	return arr.IndexOf(elem) != -1
}

// IndexOf return the index on an element in an array, else return -1
func (arr *immutableEList) IndexOf(elem interface{}) int {
	for i, value := range arr.data {
		if value == elem {
			return i
		}
	}
	return -1
}

// Iterator through the array
func (arr *immutableEList) Iterator() EIterator {
	return &listIterator{list: arr}
}

func (arr *immutableEList) ToArray() []interface{} {
	return arr.data
}
