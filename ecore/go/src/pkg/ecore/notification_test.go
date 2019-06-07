package ecore

import (
	"testing"
)

type customTestAdapterMerge struct {
	Adapter
}

var notif = NewArrayEList([]interface{}{})

func (adapter *customTestAdapterMerge) NotifyChanged(notification ENotification) {
	notif.Add(notification)
}

func TestNotificationMerge(t *testing.T) {
	testClassifier := NewEClassifierImpl()

	adapter := &customTestAdapterMerge{Adapter: *NewAdapter()}
	testClassifier.Notifier.EAdapters().Add(adapter)
	testClassifier.SetClassifierID(5)
	check(t, notif.Size(), 1, "Notification count")
	check(t, notif.Get(0).(ENotification).GetOldValue(), 0, "Notification old value")
	check(t, notif.Get(0).(ENotification).GetNewValue(), 5, "Notification new value")
	testClassifier.SetClassifierID(9)
	check(t, notif.Size(), 2, "Notification count")
	firstNotif := notif.Get(0).(ENotification)
	check(t, firstNotif.Merge(notif.Get(1).(ENotification)), true, "Notification merge")
	check(t, firstNotif.GetNewValue(), 9, "Notification new value after merge")
}
