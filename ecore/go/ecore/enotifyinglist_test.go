package ecore

import (
	"reflect"
	"testing"

	mock "github.com/stretchr/testify/mock"
)

func CreateTestNotifyingArray(mockOwner *MockEObjectInternal) *ENotifyingListImpl {
	mockOwner.On("ENotify", mock.Anything).Return()
	templateArr := NewENotifyingListImpl(mockOwner, -1)
	templateArr.Add(10)
	return templateArr
}

func TestENotifyingList(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 3 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Add(3)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 1 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.AddAll(NewArrayEList([]interface{}{1}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), []interface{}{1, 2, 3}) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD_MANY
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.InsertAll(0, NewArrayEList([]interface{}{1, 2, 3}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 3 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Insert(0, 3)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), 1) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.InsertAll(0, NewArrayEList([]interface{}{1}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), []interface{}{1, 2, 3}) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD_MANY
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.InsertAll(0, NewArrayEList([]interface{}{1, 2, 3}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == nil &&
				notif.GetOldValue() == 10 &&
				notif.GetEventType() == REMOVE
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.RemoveAt(0)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == nil &&
				notif.GetOldValue() == 10 &&
				notif.GetEventType() == REMOVE
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Remove(10)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == nil &&
				notif.GetOldValue() == 10 &&
				notif.GetEventType() == REMOVE
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Clear()
	}
}
