package ecore

// EArrayList is an array of a dynamic size
type EArrayList struct {
	data []interface{}
}

// Add a new elemement to the array
func (arr *EArrayList) Add(elem interface{}) bool {
	arr.data = append(arr.data, elem)
	return true
}

// AddAll elements of an array in the current one
func (arr *EArrayList) AddAll(list EList) bool {
	for val := range list.Iterate() {
		arr.Add(val)
	}
	return true
}

// Insert an element in the array
func (arr *EArrayList) Insert(index int, elem interface{}) bool {
	if index == 0 {
		arr.data = append([]interface{}{elem}, arr.data...)
		return true
	}
	if index == arr.Size() {
		arr.Add(elem)
		return true
	}
	if index < 0 || index > arr.Size() {
		panic("Index out of bounds")
	}
	arr.Add(arr.data[arr.Size()-1])
	copy(arr.data[index:], arr.data[index-1:])
	arr.Set(index, elem)
	return true
}

// InsertAll element of an array at a given position
func (arr *EArrayList) InsertAll(index int, list EList) bool {
	if index < 0 || index > arr.Size() {
		panic("Index out of bounds")
	}
	for val := range list.Iterate() {
		arr.Insert(index, val)
		index++
	}
	return true
}

// Move an element to the given index
func (arr *EArrayList) Move(index int, elem interface{}) {
	me := arr.IndexOf(elem)
	if me == -1 {
		panic("Index out of bounds")
	}
	arr.Swap(me, index)
}

// Swap move an element from oldIndex to newIndex
func (arr *EArrayList) Swap(oldIndex, newIndex int) interface{} {
	if oldIndex < 0 || oldIndex >= arr.Size() ||
		newIndex < 0 || newIndex > arr.Size() {
		panic("Index out of bounds")
	}
	val := arr.Get(oldIndex)
	arr.RemoveAt(oldIndex)
	if newIndex > oldIndex {
		newIndex--
	}
	arr.Insert(newIndex, val)
	return val
}

// Get an element of the array
func (arr *EArrayList) Get(index int) interface{} {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	return arr.data[index]
}

// Set an element of the array
func (arr *EArrayList) Set(index int, elem interface{}) {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	arr.data[index] = elem
}

// RemoveAt remove an element at a given position
func (arr *EArrayList) RemoveAt(index int) bool {
	if index < 0 || index >= arr.Size() {
		panic("Index out of bounds")
	}
	arr.data = append(arr.data[:index], arr.data[index+1:]...)
	return true
}

// Remove an element in an array
func (arr *EArrayList) Remove(elem interface{}) bool {
	index := arr.IndexOf(elem)
	if index == -1 {
		panic("Index out of bounds")
	}
	return arr.RemoveAt(index)
}

// Size count the number of element in the array
func (arr *EArrayList) Size() int {
	return len(arr.data)
}

// Clear remove all elements of the array
func (arr *EArrayList) Clear() {
	arr.data = make([]interface{}, 0)
}

// Empty return true if the array contains 0 element
func (arr *EArrayList) Empty() bool {
	return arr.Size() == 0
}

// Contains return if an array contains or not an element
func (arr *EArrayList) Contains(elem interface{}) bool {
	return arr.IndexOf(elem) != -1
}

// IndexOf return the index on an element in an array, else return -1
func (arr *EArrayList) IndexOf(elem interface{}) int {
	index := 0
	for val := range arr.Iterate() {
		if val == elem {
			return index
		}
		index++
	}
	return -1
}

// Iterate through the array
func (arr *EArrayList) Iterate() chan interface{} {
	ch := make(chan interface{})
	go func() {
		defer close(ch)
		for i := 0; i < arr.Size(); i++ {
			ch <- arr.Get(i)
		}
	}()
	return ch
}
