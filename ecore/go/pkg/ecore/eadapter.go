package ecore

// EAdapter is a receiver of notifications.
// An EAdapter is typically associated with a Notifier
type EAdapter interface {
	// NotifyChanged Notifies that a change to some feature has occurred.
	NotifyChanged(notification ENotification)

	// GetTarget Returns the target from which the adapter receives notification.
	GetTarget() ENotifier

	// SetTarget Sets the target from which the adapter will receive notification.
	SetTarget(ENotifier)
}
