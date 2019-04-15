package ecore

// EList is the interface for dynamic containers
type EList interface {
	Add(interface{})

	AddAll(EList)

	Insert(int, interface{})

	InsertAll(int, EList) bool

	Move(int, interface{})

	Order(int, int) interface{}

	Get(int) interface{}

	Set(int, interface{})

	RemoveAt(int) interface{}

	Remove(interface{}) bool

	Size() int

	Clear()

	Empty()

	Contains(interface{}) bool

	IndexOf(interface{}) int

	Iterate() chan interface{}
}
