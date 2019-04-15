package ecore

// EList is the interface for dynamic containers
type EList interface {
	Add(interface{}) bool

	AddAll(EList) bool

	Insert(uint64, interface{}) bool

	InsertAll(uint64, EList) bool

	Move(uint64, interface{})

	Order(uint64, uint64) interface{}

	Get(uint64) interface{}

	Set(uint64, interface{})

	RemoveAt(uint64) interface{}

	Remove(interface{}) bool

	Size() uint64

	Clear()

	Empty()

	Contains(interface{}) bool

	IndexOf(interface{}) uint64
}
