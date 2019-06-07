package ecore

type EAdapter interface {
	NotifyChanged(notification ENotification)

	GetTarget() ENotifier

	SetTarget(ENotifier)
}
