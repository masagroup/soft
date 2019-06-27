package ecore

type notification struct {
	eventType EventType
	oldValue  interface{}
	newValue  interface{}
	position  int
	next      ENotificationChain
	notifier  EObject
	feature   EStructuralFeature
	featureID int
}

// NewNotificationByFeature ...
func NewNotificationByFeature(notifier EObject, eType EventType, feature EStructuralFeature, oldValue interface{}, newValue interface{}, position int) ENotification {
	return &notification{
		eventType: eType,
		oldValue:  oldValue,
		newValue:  newValue,
		position:  position,
		notifier:  notifier,
		feature:   feature,
		featureID: -1,
	}
}

// NewNotificationByFeatureID ...
func NewNotificationByFeatureID(notifier EObject, eType EventType, featureID int, oldValue interface{}, newValue interface{}, position int) ENotification {
	return &notification{
		eventType: eType,
		oldValue:  oldValue,
		newValue:  newValue,
		position:  position,
		notifier:  notifier,
		featureID: featureID,
	}
}

func (notif *notification) GetEventType() EventType {
	return notif.eventType
}

func (notif *notification) GetOldValue() interface{} {
	return notif.oldValue
}

func (notif *notification) GetNewValue() interface{} {
	return notif.newValue
}

func (notif *notification) GetPosition() int {
	return notif.position
}

func (notif *notification) GetNotifier() ENotifier {
	return notif.notifier.(ENotifier)
}

func (notif *notification) GetFeature() EStructuralFeature {
	if notif.feature != nil {
		return notif.feature
	}
	return notif.notifier.EClass().GetEStructuralFeature(notif.featureID)
}

func (notif *notification) GetFeatureID() int {
	if notif.featureID != -1 {
		return notif.featureID
	}
	if notif.feature != nil {
		return notif.feature.GetFeatureID()
	}
	return -1
}

func (notif *notification) Merge(eNotif ENotification) bool {
	switch ev := notif.eventType; ev {
	case SET, UNSET:
		switch notifEv := eNotif.GetEventType(); notifEv {
		case SET, UNSET:
			if notif.GetNotifier() == eNotif.GetNotifier() &&
				notif.GetFeatureID() == eNotif.GetFeatureID() {
				notif.newValue = eNotif.GetNewValue()
				if eNotif.GetEventType() == SET {
					notif.eventType = SET
					return true
				}
			}
		}
	case REMOVE:
		switch notifEv := eNotif.GetEventType(); notifEv {
		case REMOVE:
			if notif.GetNotifier() == eNotif.GetNotifier() &&
				notif.GetFeatureID() == eNotif.GetFeatureID() {
				originalPosition := notif.GetPosition()
				notificationPosition := eNotif.GetPosition()
				notif.eventType = REMOVE_MANY
				removedValues := NewArrayEList([]interface{}{})
				if originalPosition <= notificationPosition {
					removedValues.Add(notif.oldValue)
					removedValues.Add(eNotif.GetOldValue())
					notif.position = originalPosition
					notif.newValue = NewArrayEList([]interface{}{originalPosition, notificationPosition + 1})
				} else {
					removedValues.Add(eNotif.GetOldValue())
					removedValues.Add(notif.oldValue)
					notif.position = notificationPosition
					notif.newValue = NewArrayEList([]interface{}{notificationPosition, originalPosition})
				}
				notif.oldValue = removedValues
				return true
			}
		}
	case REMOVE_MANY:
		switch notifEv := eNotif.GetEventType(); notifEv {
		case REMOVE:
			if notif.GetNotifier() == eNotif.GetNotifier() &&
				notif.GetFeatureID() == eNotif.GetFeatureID() {
				notificationPosition := eNotif.GetPosition()
				positions := notif.newValue.(EList)
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
				notif.oldValue.(EList).Insert(index, eNotif.GetOldValue())
				newPositions.Set(index, notificationPosition)
				index++
				for ; index < positions.Size()+1; index++ {
					newPositions.Set(index, positions.Get(index-1))
				}
				notif.newValue = newPositions
				return true
			}
		}
	}
	return false
}

func (notif *notification) Add(eNotif ENotification) bool {
	if eNotif == nil {
		return false
	}
	if notif.Merge(eNotif) {
		return false
	}
	if notif.next == nil {
		value, ok := eNotif.(ENotificationChain)
		if ok {
			notif.next = value
			return true
		} else {
			notif.next = &NotificationChain{}
			return notif.next.Add(eNotif)
		}
	} else {
		return notif.next.Add(eNotif)
	}
}

func (notif *notification) Dispatch() {
	notifier := notif.GetNotifier()
	if notifier != nil && notif.eventType != -1 {
		notifier.ENotify(notif)
	}
	if notif.next != nil {
		notif.next.Dispatch()
	}
}
