package ecore

// BasicEObject is a basic implementation of an EObject
type BasicEObject struct {
	*Notifier
	internal interface{}
}

// NewBasicEObject is BasicEObject constructor
func NewBasicEObject() *BasicEObject {
	basicEObject := new(BasicEObject)
	basicEObject.Notifier = NewNotifier()
	return basicEObject
}
