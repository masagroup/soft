package ecore

// arrayEList is an array of a dynamic size
type arrayEList struct {
	data     []interface{}
	internal interface{}
}

type immutableEList struct {
	data []interface{}
}

// NewArrayEList return a new ArrayEList
func NewArrayEList(data []interface{}) *arrayEList {
	return &arrayEList{
		data:     data,
		internal: nil,
	}
}

// NewImmutableEList return a new ImmutableEList
func NewImmutableEList(data []interface{}) *immutableEList {
	return &immutableEList{data: data}
}

type iterator struct {
	curr int
	data EList
}

// Value return the current value of the iterator
func (it *iterator) Value() interface{} {
	return it.data.Get(it.curr)
}

// Next make the iterator go further in the array
func (it *iterator) Next() bool {
	it.curr++
	return it.curr != it.data.Size()
}

// Add a new element to the array
func (arr *arrayEList) Add(elem interface{}) bool {
	if arr.internal == nil {
		arr.data = append(arr.data, elem)
		return true
	}
	return arr.internal.(EListInternal).Add(elem)
}

// AddAll elements of an array in the current one
func (arr *arrayEList) AddAll(list EList) bool {
	if arr.internal == nil {
		arr.data = append(arr.data, list.ToArray()...)
		return true
	}
	return arr.internal.(EListInternal).AddAll(list)
}

// Insert an element in the array
func (arr *arrayEList) Insert(index int, elem interface{}) bool {
	if arr.internal == nil {
		if index < 0 || index > arr.Size() {
			panic("Index out of bounds")
		}
		arr.data = append(arr.data, nil)
		copy(arr.data[index+1:], arr.data[index:])
		arr.data[index] = elem
		return true
	}
	return arr.internal.(EListInternal).Insert(index, elem)
}

// InsertAll element of an array at a given position
func (arr *arrayEList) InsertAll(index int, list EList) bool {
	if arr.internal == nil {
		if index < 0 || index > arr.Size() {
			panic("Index out of bounds")
		}
		arr.data = append(arr.data[:index], append(list.ToArray(), arr.data[index:]...)...)
		return true
	}
	return arr.internal.(EListInternal).InsertAll(index, list)
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
		panic("Index out of bounds")
	}
	val := arr.data[oldIndex]
	copy(arr.data[oldIndex:], arr.data[oldIndex+1:])
	if newIndex > oldIndex {
		newIndex--
	}
	copy(arr.data[newIndex+1:], arr.data[newIndex:])
	arr.data[newIndex] = val
	return val
}

// Get an element of the array
func (arr *arrayEList) Get(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	return arr.data[index]
}

// Set an element of the array
func (arr *arrayEList) Set(index int, elem interface{}) {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	arr.data[index] = elem
}

// RemoveAt remove an element at a given position
func (arr *arrayEList) RemoveAt(index int) bool {
	if arr.internal == nil {
		if index < 0 || index >= arr.Size() {
			panic("Index out of bounds")
		}
		arr.data = append(arr.data[:index], arr.data[index+1:]...)
		return true
	}
	return arr.internal.(EListInternal).RemoveAt(index)
}

// Remove an element in an array
func (arr *arrayEList) Remove(elem interface{}) bool {
	if arr.internal == nil {
		index := arr.IndexOf(elem)
		if index == -1 {
			return false
		}
		return arr.RemoveAt(index)
	}
	return arr.internal.(EListInternal).Remove(elem)
}

// Size count the number of element in the array
func (arr *arrayEList) Size() int {
	return len(arr.data)
}

// Clear remove all elements of the array
func (arr *arrayEList) Clear() {
	if arr.internal == nil {
		arr.data = make([]interface{}, 0)
	}
	arr.internal.(EListInternal).Clear()
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

// Iterate through the array
func (arr *arrayEList) Iterate() EIterator {
	return &iterator{data: arr, curr: -1}
}

func (arr *arrayEList) ToArray() []interface{} {
	return arr.data
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
		panic("Index out of bounds")
	}
	return arr.data[index]
}

func (arr *immutableEList) Set(index int, elem interface{}) {
	panic("Immutable list can't be modified")
}

func (arr *immutableEList) RemoveAt(index int) bool {
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

// Iterate through the array
func (arr *immutableEList) Iterate() EIterator {
	return &iterator{data: arr, curr: -1}
}

func (arr *immutableEList) ToArray() []interface{} {
	return arr.data
}
