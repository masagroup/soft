package ecore

type NotificationChain struct {
	notifications_ EList
}

func (notif *NotificationChain) Add(newNotif ENotification) bool {
	if newNotif == nil {
		return false
	}
	for it := notif.notifications_.Iterate(); it.Next(); {
		value := it.Value().(ENotification)
		if value != nil && value.Merge(newNotif) {
			return false
		}
	}
	notif.notifications_.Add(newNotif)
	return true
}

func (notif *NotificationChain) Dispatch() {
	for it := notif.notifications_.Iterate(); it.Next(); {
		value := it.Value().(ENotification)
		notifier := value.GetNotifier()
		if notifier != nil && value.GetEventType() != -1 {
			notifier.ENotify(value)
		}
	}
}
