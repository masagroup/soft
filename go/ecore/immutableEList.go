package ecore

import "strconv"

type immutableEList struct {
	data []interface{}
}

// NewImmutableEList return a new ImmutableEList
func NewImmutableEList(data []interface{}) *immutableEList {
	return &immutableEList{data: data}
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
