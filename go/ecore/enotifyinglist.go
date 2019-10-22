package ecore

// ENotifyingList ...
type ENotifyingList interface {
	EList

	GetNotifier() ENotifier

	GetFeature() EStructuralFeature

	GetFeatureID() int

	AddWithNotification(object interface{}, notifications ENotificationChain) ENotificationChain

	RemoveWithNotification(object interface{}, notifications ENotificationChain) ENotificationChain

	SetWithNotification(index int, object interface{}, notifications ENotificationChain) ENotificationChain
}
