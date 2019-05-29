package ecore

type EAdapter interface {
	GetTarget() ENotifier

	SetTarget(ENotifier)
}
