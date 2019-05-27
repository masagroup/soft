package ecore

type AbstractNotification struct {
	ENotification
	eventType_ EventType
	oldValue_  interface{}
	newValue_  interface{}
	position_  int
	next_      ENotificationChain
}

func (notif *AbstractNotification) GetEventType() EventType {
	return notif.eventType_
}

func (notif *AbstractNotification) GetOldValue() interface{} {
	return notif.oldValue_
}

func (notif *AbstractNotification) GetNewValue() interface{} {
	return notif.newValue_
}

func (notif *AbstractNotification) GetPosition() int {
	return notif.position_
}

func (notif *AbstractNotification) Merge(eNotif ENotification) bool {
	switch ev := notif.eventType_; ev {
	case SET:
	case UNSET:
		switch notifEv := eNotif.GetEventType(); notifEv {
		case SET:
		case UNSET:
			if notif.ENotification.GetNotifier() == eNotif.GetNotifier() &&
				notif.ENotification.GetFeatureId() == eNotif.GetFeatureId() {
				notif.newValue_ = eNotif.GetNewValue()
				if notif.GetEventType() == SET {
					notif.eventType_ = SET
					return true
				}
			}
		}
	case REMOVE:
		switch notifEv := eNotif.GetEventType(); notifEv {
		case REMOVE:
			if notif.ENotification.GetNotifier() == eNotif.GetNotifier() &&
				notif.ENotification.GetFeatureId() == eNotif.GetFeatureId() {
				originalPosition := notif.ENotification.GetPosition()
				notificationPosition := eNotif.GetPosition()
				notif.eventType_ = REMOVE_MANY
				removedValues := NewArrayEList([]interface{}{})
				if originalPosition <= notificationPosition {
					removedValues.Add(notif.oldValue_)
					removedValues.Add(eNotif.GetOldValue())
					notif.position_ = originalPosition
					notif.newValue_ = NewArrayEList([]interface{}{originalPosition, notificationPosition + 1})
				} else {
					removedValues.Add(eNotif.GetOldValue())
					removedValues.Add(notif.oldValue_)
					notif.position_ = notificationPosition
					notif.newValue_ = NewArrayEList([]interface{}{notificationPosition, originalPosition})
				}
				notif.oldValue_ = removedValues
				return true
			}
		}
	case REMOVE_MANY:
		switch notifEv := eNotif.GetEventType(); notifEv {
		case REMOVE:
			if notif.ENotification.GetNotifier() == eNotif.GetNotifier() &&
				notif.ENotification.GetFeatureId() == eNotif.GetFeatureId() {
				notificationPosition := eNotif.GetPosition()
				positions := notif.newValue_.(EList)
				newPositions := NewArrayEList([]interface{}{})
				for i := 0; i < positions.Size()+1; i++ {
					newPositions.Add(0)
				}
				index := 0
				for index < positions.Size() {
					oldPosition := positions.Get(index).(int)
					if oldPosition <= notificationPosition {
						newPositions.Set(index, oldPosition)
						index++
					} else {
						break
					}
				}
				notif.oldValue_.(EList).Insert(index, eNotif.GetOldValue())
				newPositions.Set(index, notificationPosition)
				index++
				for ; index < positions.Size()+1; index++ {
					newPositions.Set(index, positions.Get(index-1))
				}
				notif.newValue_ = newPositions
				return true
			}
		}
	}
	return false
}

func (notif *AbstractNotification) Add(eNotif ENotification) bool {
	if eNotif == nil {
		return false
	}
	if notif.Merge(eNotif) {
		return false
	}
	if notif.next_ == nil {
		value, ok := eNotif.(ENotificationChain)
		if ok {
			notif.next_ = value
			return true
		} else {
			notif.next_ = &NotificationChain{}
			return notif.next_.Add(eNotif)
		}
	} else {
		return notif.next_.Add(eNotif)
	}
}

func (notif *AbstractNotification) Dispatch() {
	notifier := notif.ENotification.GetNotifier()
	if notifier != nil && notif.eventType_ != -1 {
		notifier.ENotify(notif)
	}
	if notif.next_ != nil {
		notif.next_.Dispatch()
	}
}
