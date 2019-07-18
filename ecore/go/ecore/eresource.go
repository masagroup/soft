package ecore

//EResource ...
type EResource interface {
	GetContents() EList

	Attached(object EObject)
	Detached(object EObject)
}
