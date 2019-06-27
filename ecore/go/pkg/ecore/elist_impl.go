package ecore

// arrayEList is an array of a dynamic size
type arrayEList struct {
	abstractEList
	data     []interface{}
	isUnique bool
}

type immutableEList struct {
	data []interface{}
}

// NewEmptyArrayEList return a new ArrayEList
func NewEmptyArrayEList() *arrayEList {
	return &arrayEList{data: []interface{}{}}
}

// NewArrayEList return a new ArrayEList
func NewArrayEList(data []interface{}) *arrayEList {
	return &arrayEList{
		data:     data,
		isUnique: false,
	}
}

// NewUniqueArrayEList return a new ArrayEList with isUnique set as true
func NewUniqueArrayEList(data []interface{}) *arrayEList {
	return &arrayEList{
		data:     data,
		isUnique: true,
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

// Remove all elements in list that already are in arr.data
func (arr *arrayEList) removeDuplicated(list EList) *arrayEList {
	newArr := NewArrayEList([]interface{}{})
	for it := list.Iterate(); it.Next(); {
		if !newArr.Contains(it.Value()) && !arr.Contains(it.Value()) {
			newArr.Add(it.Value())
		}
	}
	return newArr
}

// Add a new element to the array
func (arr *arrayEList) doAdd(elem interface{}) {
	arr.data = append(arr.data, elem)
}

func (arr *arrayEList) Add(elem interface{}) bool {
	if arr.isUnique && arr.Contains(elem) {
		return false
	}
	arr.doAdd(elem)
	return true
}

func (arr *arrayEList) doAddAll(list EList) {
	arr.data = append(arr.data, list.ToArray()...)
}

// AddAll elements of an array in the current one
func (arr *arrayEList) AddAll(list EList) bool {
	if arr.isUnique {
		list = arr.removeDuplicated(list)
		if list.Size() == 0 {
			return false
		}
	}
	arr.doAddAll(list)
	return true
}

func (arr *arrayEList) doInsert(index int, elem interface{}) {
	arr.data = append(arr.data, nil)
	copy(arr.data[index+1:], arr.data[index:])
	arr.data[index] = elem
}

// Insert an element in the array
func (arr *arrayEList) Insert(index int, elem interface{}) bool {
	if index < 0 || index > arr.Size() {
		panic("Index out of bounds")
	}
	if arr.isUnique && arr.Contains(elem) {
		return false
	}
	arr.doInsert(index, elem)
	return true
}

func (arr *arrayEList) doInsertAll(index int, list EList) {
	arr.data = append(arr.data[:index], append(list.ToArray(), arr.data[index:]...)...)
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
	if arr.isUnique {
		list = arr.removeDuplicated(list)
		if list.Size() == 0 {
			return false
		}
	}
	arr.doInsertAll(index, list)
	return true
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

func (arr *arrayEList) doSet(index int, elem interface{}) {
	arr.data[index] = elem
}

// Set an element of the array
func (arr *arrayEList) Set(index int, elem interface{}) { // TODO
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
		if !arr.Contains(elem) {
			arr.doSet(index, elem)
		}
	}
}

// RemoveAt remove an element at a given position
func (arr *arrayEList) RemoveAt(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	elem := arr.Get(index)
	arr.data = append(arr.data[:index], arr.data[index+1:]...)
	return elem
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
	arr.RemoveAt(index)
	return true
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

// Iterate through the array
func (arr *immutableEList) Iterate() EIterator {
	return &iterator{data: arr, curr: -1}
}

func (arr *immutableEList) ToArray() []interface{} {
	return arr.data
}
