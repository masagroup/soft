package ecore

type ENotifier interface {
	EAdapters() EList

	EDeliver() bool

	ESetDeliver(bool)

	ENotify(ENotification)
}
