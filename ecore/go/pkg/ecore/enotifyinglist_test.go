package ecore

import (
	"reflect"
	"testing"

	mock "github.com/stretchr/testify/mock"
)

func TestENotifyingList(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 3 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := NewENotifyingListImpl(mockOwner, -1)
		arr.Add(3)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 1 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := NewENotifyingListImpl(mockOwner, -1)
		arr.AddAll(NewArrayEList([]interface{}{1}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), []interface{}{1, 2, 3}) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD_MANY
		})).Return()
		arr := NewENotifyingListImpl(mockOwner, -1)
		arr.AddAll(NewArrayEList([]interface{}{1, 2, 3}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 3 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := NewENotifyingListImpl(mockOwner, -1)
		arr.Insert(0, 3)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), []interface{}{1, 2, 3}) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD_MANY
		})).Return()
		arr := NewENotifyingListImpl(mockOwner, -1)
		arr.InsertAll(0, NewArrayEList([]interface{}{1, 2, 3}))
	}
}
