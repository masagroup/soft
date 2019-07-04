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

func TestNotificationDispatch( t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	notification  := NewNotificationByFeature(mockObject, ADD, mockFeature, 1, 2, NO_INDEX)
	mockObject.On("ENotify",notification).Once()
	notification.Dispatch()
}

func TestNotificationMerge( t *testing.T) {
	mockObject := &MockEObject{}
	mockFeature := &MockEStructuralFeature{}
	mockFeature.On("GetFeatureID").Return(1)
	{
		notification  := NewNotificationByFeatureID(mockObject, SET, 1 , 1, 2, NO_INDEX)
		other := NewNotificationByFeatureID(mockObject, SET, 1 , 2, 3, NO_INDEX)
		assert.True( t,  notification.Merge( other ) )
		assert.Equal( t, SET , notification.GetEventType() )
		assert.Equal( t, 1 , notification.GetOldValue() )
		assert.Equal( t, 3 , notification.GetNewValue() )
	}
	// {
	// 	notification := NewNotificationByFeature( mockObject, SET, mockFeature, 1, 2 , NO_INDEX );
    //     other := NewNotificationByFeature( mockObject, UNSET, mockFeature, 2, 0 , NO_INDEX );
    //     assert.True( t,  notification.Merge( other ) )
    //     assert.Equal( t, SET , notification.GetEventType() )
	// 	assert.Equal( t, 1 , notification.GetOldValue() )
	// 	assert.Equal( t, 0 , notification.GetNewValue() )
	// }
}

func TestNotificationAdd( t *testing.T) {

}

