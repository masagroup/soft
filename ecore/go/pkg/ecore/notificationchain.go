package ecore

// NotificationChain is an implementation of ENotificationChain interface
type NotificationChain struct {
	notifications *arrayEList
}

func NewNotificationChain() *NotificationChain {
<<<<<<< HEAD
	return &NotificationChain{notifications: NewEmptyArrayEList()}
=======
	return &NotificationChain{notifications: NewArrayEList([]interface{}{})}
>>>>>>> fc0273af33d07dd89397841c1ba1b199d8655a81
}

// Add Adds a notification to the chain.
func (notif *NotificationChain) Add(newNotif ENotification) bool {
	if newNotif == nil {
		return false
	}
	for it := notif.notifications.Iterate(); it.Next(); {
		if it.Value().(ENotification).Merge(newNotif) {
			return false
		}
	}
	notif.notifications.Add(newNotif)
	return true
}

// Dispatch Dispatches each notification to the appropriate notifier via notifier.ENotify method
func (notif *NotificationChain) Dispatch() {
	for it := notif.notifications.Iterate(); it.Next(); {
		value := it.Value().(ENotification)
		notifier := value.GetNotifier()
		if notifier != nil && value.GetEventType() != -1 {
			notifier.ENotify(value)
		}
	}
}
