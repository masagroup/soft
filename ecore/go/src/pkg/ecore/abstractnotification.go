package ecore

type AbstractNotification struct {
	ENotification // TODO
	eventType     EventType
	oldValue      interface{}
	newValue      interface{}
	position      int
	next          ENotificationChain
}

func (notif *AbstractNotification) GetEventType() EventType {
	return notif.eventType
}

func (notif *AbstractNotification) GetOldValue() interface{} {
	return notif.oldValue
}

func (notif *AbstractNotification) GetNewValue() interface{} {
	return notif.newValue
}

func (notif *AbstractNotification) GetPosition() int {
	return notif.position
}

func (notif *AbstractNotification) Merge(eNotif ENotification) bool {
	// TODO
	return false
}

func (notif *AbstractNotification) Add(eNotif ENotification) bool {
	if eNotif == nil {
		return false
	}
	if notif.Merge(eNotif) {
		return false
	}
	if notif.next == nil {
		// TODO
	} else {
		return notif.next.Add(eNotif)
	}
	return false
}

func (notif *AbstractNotification) Dispatch() {
	notifier := notif.ENotification.GetNotifier()
	if notifier != nil && notif.eventType != -1 {
		notifier.ENotify(notif)
	}
	if notif.next != nil {
		notif.next.Dispatch()
	}
}
