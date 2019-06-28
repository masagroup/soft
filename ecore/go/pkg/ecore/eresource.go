package ecore

type EResource interface {
	Attached( object EObject)
	Detached( object EObject)
}