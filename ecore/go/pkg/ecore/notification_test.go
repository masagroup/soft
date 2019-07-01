package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

type customTestAdapterMerge struct {
	Adapter
}

var notif *arrayEList

func (adapter *customTestAdapterMerge) NotifyChanged(notification ENotification) {
	notif.Add(notification)
}

func TestNotificationMerge(t *testing.T) {
	testClassifier := newEClassifierImpl()
	notif = NewArrayEList([]interface{}{})

	adapter := &customTestAdapterMerge{Adapter: *NewAdapter()}
	testClassifier.EAdapters().Add(adapter)
	testClassifier.SetClassifierID(5)
	assert.Equal(t, notif.Size(), 1, "Notification count")
	assert.Equal(t, notif.Get(0).(ENotification).GetOldValue(), -1)
	assert.Equal(t, notif.Get(0).(ENotification).GetNewValue(), 5)
	testClassifier.SetClassifierID(9)
	assert.Equal(t, notif.Size(), 2, "Notification count")
	firstNotif := notif.Get(0).(ENotification)
	assert.Equal(t, firstNotif.Merge(notif.Get(1).(ENotification)), true)
	assert.Equal(t, firstNotif.GetNewValue(), 9)
}

func TestNotificationListAdd(t *testing.T) {
	arr := NewENotifyingListImpl(nil, -1)
	notif = NewArrayEList([]interface{}{})

	adapter := &customTestAdapterMerge{Adapter: *NewAdapter()}
	arr.notifier.EAdapters().Add(adapter)
	arr.Add(3)
	assert.Equal(t, notif.Size(), 1, "Notification count")
	assert.Equal(t, notif.Get(0).(ENotification).GetNewValue(), 3)
}
