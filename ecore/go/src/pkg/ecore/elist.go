package ecore

// EList is the interface for dynamic containers
type EList interface {
	Add(interface{}) bool

	AddAll(EList) bool

	Insert(int, interface{}) bool

	InsertAll(int, EList) bool

	Move(int, interface{})

	Swap(int, int) interface{}

	Get(int) interface{}

	Set(int, interface{})

	RemoveAt(int) bool

	Remove(interface{}) bool

	Size() int

	Clear()

	Empty() bool

	Contains(interface{}) bool

	IndexOf(interface{}) int

	Iterate() chan interface{}
}
