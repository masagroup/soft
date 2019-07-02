package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNotificationListAdd(t *testing.T) {
	arr := NewENotifyingListImpl(NewBasicEObject(), -1)
	notif = NewArrayEList([]interface{}{})

	adapter := &customTestAdapterMerge{Adapter: *NewAdapter()}
	arr.owner.EAdapters().Add(adapter)
	arr.Add(3)
	assert.Equal(t, notif.Size(), 1, "Notification count")
	assert.Equal(t, notif.Get(0).(ENotification).GetNewValue(), 3)
	assert.Equal(t, notif.Get(0).(ENotification).GetEventType(), ADD)
}

func TestNotificationListAddAll(t *testing.T) {
	arr := NewENotifyingListImpl(NewBasicEObject(), -1)
	arr2 := NewArrayEList([]interface{}{3, 2})
	notif = NewArrayEList([]interface{}{})

	adapter := &customTestAdapterMerge{Adapter: *NewAdapter()}
	arr.owner.EAdapters().Add(adapter)
	arr.AddAll(arr2)
	assert.Equal(t, notif.Size(), 1, "Notification count")
	assert.Equal(t, notif.Get(0).(ENotification).GetNewValue(), []interface{}{3, 2})
	assert.Equal(t, notif.Get(0).(ENotification).GetEventType(), ADD_MANY)
}

func TestNotificationListAddAllOne(t *testing.T) {
	arr := NewENotifyingListImpl(NewBasicEObject(), -1)
	arr2 := NewArrayEList([]interface{}{2, 2})
	notif = NewArrayEList([]interface{}{})

	adapter := &customTestAdapterMerge{Adapter: *NewAdapter()}
	arr.owner.EAdapters().Add(adapter)
	arr.AddAll(arr2)
	assert.Equal(t, notif.Size(), 1, "Notification count")
	assert.Equal(t, notif.Get(0).(ENotification).GetNewValue(), 2)
	assert.Equal(t, notif.Get(0).(ENotification).GetEventType(), ADD)
}
