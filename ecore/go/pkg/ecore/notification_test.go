package ecore

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestNotificationConstructor(t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	{
		notification  := NewNotificationByFeature(mockObject, ADD, mockFeature, 1,2, NO_INDEX)
		assert.Equal( t , notification.GetNotifier() , mockObject )
		assert.Equal( t , notification.GetEventType() , ADD )
		assert.Equal( t , notification.GetFeature() , mockFeature )
		assert.Equal( t , notification.GetOldValue() , 1 )
		assert.Equal( t , notification.GetNewValue() , 2 )
		assert.Equal( t , notification.GetPosition() , NO_INDEX )
	}
	{
		notification  := NewNotificationByFeature(mockObject, REMOVE, mockFeature, 1, 2, 3)
		assert.Equal( t , notification.GetNotifier() , mockObject )
		assert.Equal( t , notification.GetEventType() , REMOVE )
		assert.Equal( t , notification.GetFeature() , mockFeature )
		assert.Equal( t , notification.GetOldValue() , 1 )
		assert.Equal( t , notification.GetNewValue() , 2 )
		assert.Equal( t , notification.GetPosition() , 3 )
	}
	{
		notification  := NewNotificationByFeatureID(mockObject, ADD_MANY, 10, 1, 2, NO_INDEX)
		assert.Equal( t , notification.GetNotifier() , mockObject )
		assert.Equal( t , notification.GetEventType() , ADD_MANY )
		assert.Equal( t , notification.GetFeatureID() , 10 )
		assert.Equal( t , notification.GetOldValue() , 1 )
		assert.Equal( t , notification.GetNewValue() , 2 )
		assert.Equal( t , notification.GetPosition() , NO_INDEX )
	}
	{
		notification  := NewNotificationByFeatureID(mockObject, REMOVE_MANY, 10, 1, 2, 3)
		assert.Equal( t , notification.GetNotifier() , mockObject )
		assert.Equal( t , notification.GetEventType() , REMOVE_MANY )
		assert.Equal( t , notification.GetFeatureID() , 10 )
		assert.Equal( t , notification.GetOldValue() , 1 )
		assert.Equal( t , notification.GetNewValue() , 2 )
		assert.Equal( t , notification.GetPosition() , 3 )
	}
}

func TestNotificationGetFeatureID(t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	{
		notification  := NewNotificationByFeatureID(mockObject, REMOVE, 10, 1, 2, 3)
		assert.Equal( t , notification.GetFeatureID() , 10 )
	}
	{
		notification  := NewNotificationByFeature(mockObject, ADD, mockFeature, 1,2, NO_INDEX)
		mockFeature.On("GetFeatureID").Return(5)
		assert.Equal( t , notification.GetFeatureID() , 5 )
	}
	

}

func TestNotificationGetFeature(t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	mockClass := &MockEClass{}
	{
		notification  := NewNotificationByFeature(mockObject, ADD_MANY, mockFeature, 1, 2, NO_INDEX)
		assert.Equal( t , notification.GetFeature() , mockFeature )
	}
	{
		notification  := NewNotificationByFeatureID(mockObject, REMOVE_MANY, 10, 1, 2, 3)
		mockObject.On("EClass").Return(mockClass)
		mockClass.On("GetEStructuralFeature",10).Return(mockFeature)
		assert.Equal( t , notification.GetFeature() , mockFeature )
	}
}


type customTestAdapterMerge struct {
	Adapter
}

var notif = NewArrayEList([]interface{}{})

func (adapter *customTestAdapterMerge) NotifyChanged(notification ENotification) {
	notif.Add(notification)
}

func TestNotificationMerge(t *testing.T) {
	testClassifier := newEClassifierImpl()

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
