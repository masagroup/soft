package ecore

import (
	"github.com/stretchr/testify/assert"
	"net/url"
	"testing"
)

func TestBasicEObjectGetInterfaces(t *testing.T) {
	o := NewBasicEObject()
	assert.Equal(t, o, o.GetInterfaces())
}

func TestBasicEObjectGetEObject(t *testing.T) {
	o := NewBasicEObject()
	assert.Equal(t, o, o.AsEObject())
}

func TestBasicEObjectEClass(t *testing.T) {
	o := NewBasicEObject()
	assert.Equal(t, GetPackage().GetEObject(), o.EClass())
}

func TestBasicEObjectEIsProxy(t *testing.T) {
	o := NewBasicEObject()
	assert.False(t, o.EIsProxy())
	o.ESetProxyURI(&url.URL{})
	assert.True(t, o.EIsProxy())
}

func TestBasicEObjectContainer(t *testing.T) {
	// set the container
	o := NewBasicEObject()
	mockObject := new(MockEObject)
	mockResource := new(MockEResource)
	mockObject.On("EResource").Return(mockResource)
	mockResource.On("Attached", o)
	mockNotifications := new(MockENotificationChain)
	assert.Equal(t, mockNotifications, o.EBasicSetContainer(mockObject, 1, mockNotifications))
	assert.Equal(t, mockObject, o.EContainer())
	assert.Equal(t, 1, o.EContainerFeatureID())
}
