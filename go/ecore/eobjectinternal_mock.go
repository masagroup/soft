// Code generated by mockery v1.0.0. DO NOT EDIT.

package ecore

import (
	url "net/url"

	mock "github.com/stretchr/testify/mock"
)

// MockEObjectInternal is an autogenerated mock type for the EObjectInternal type
type MockEObjectInternal struct {
	mock.Mock
}

// EAdapters provides a mock function with given fields:
func (_m *MockEObjectInternal) EAdapters() EList {
	ret := _m.Called()

	var r0 EList
	if rf, ok := ret.Get(0).(func() EList); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EList)
		}
	}

	return r0
}

// EAllContents provides a mock function with given fields:
func (_m *MockEObjectInternal) EAllContents() EIterator {
	ret := _m.Called()

	var r0 EIterator
	if rf, ok := ret.Get(0).(func() EIterator); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EIterator)
		}
	}

	return r0
}

// EBasicInverseAdd provides a mock function with given fields: otherEnd, featureID, notifications
func (_m *MockEObjectInternal) EBasicInverseAdd(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	ret := _m.Called(otherEnd, featureID, notifications)

	var r0 ENotificationChain
	if rf, ok := ret.Get(0).(func(EObject, int, ENotificationChain) ENotificationChain); ok {
		r0 = rf(otherEnd, featureID, notifications)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotificationChain)
		}
	}

	return r0
}

// EBasicInverseRemove provides a mock function with given fields: otherEnd, featureID, notifications
func (_m *MockEObjectInternal) EBasicInverseRemove(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	ret := _m.Called(otherEnd, featureID, notifications)

	var r0 ENotificationChain
	if rf, ok := ret.Get(0).(func(EObject, int, ENotificationChain) ENotificationChain); ok {
		r0 = rf(otherEnd, featureID, notifications)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotificationChain)
		}
	}

	return r0
}

// EClass provides a mock function with given fields:
func (_m *MockEObjectInternal) EClass() EClass {
	ret := _m.Called()

	var r0 EClass
	if rf, ok := ret.Get(0).(func() EClass); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EClass)
		}
	}

	return r0
}

// EContainer provides a mock function with given fields:
func (_m *MockEObjectInternal) EContainer() EObject {
	ret := _m.Called()

	var r0 EObject
	if rf, ok := ret.Get(0).(func() EObject); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EObject)
		}
	}

	return r0
}

// EContainingFeature provides a mock function with given fields:
func (_m *MockEObjectInternal) EContainingFeature() EStructuralFeature {
	ret := _m.Called()

	var r0 EStructuralFeature
	if rf, ok := ret.Get(0).(func() EStructuralFeature); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EStructuralFeature)
		}
	}

	return r0
}

// EContainmentFeature provides a mock function with given fields:
func (_m *MockEObjectInternal) EContainmentFeature() EReference {
	ret := _m.Called()

	var r0 EReference
	if rf, ok := ret.Get(0).(func() EReference); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EReference)
		}
	}

	return r0
}

// EContents provides a mock function with given fields:
func (_m *MockEObjectInternal) EContents() EList {
	ret := _m.Called()

	var r0 EList
	if rf, ok := ret.Get(0).(func() EList); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EList)
		}
	}

	return r0
}

// ECrossReferences provides a mock function with given fields:
func (_m *MockEObjectInternal) ECrossReferences() EList {
	ret := _m.Called()

	var r0 EList
	if rf, ok := ret.Get(0).(func() EList); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EList)
		}
	}

	return r0
}

// EDeliver provides a mock function with given fields:
func (_m *MockEObjectInternal) EDeliver() bool {
	ret := _m.Called()

	var r0 bool
	if rf, ok := ret.Get(0).(func() bool); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// EDirectResource provides a mock function with given fields:
func (_m *MockEObjectInternal) EDirectResource() EResource {
	ret := _m.Called()

	var r0 EResource
	if rf, ok := ret.Get(0).(func() EResource); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EResource)
		}
	}

	return r0
}

// EGet provides a mock function with given fields: _a0
func (_m *MockEObjectInternal) EGet(_a0 EStructuralFeature) interface{} {
	ret := _m.Called(_a0)

	var r0 interface{}
	if rf, ok := ret.Get(0).(func(EStructuralFeature) interface{}); ok {
		r0 = rf(_a0)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(interface{})
		}
	}

	return r0
}

// EGetFromID provides a mock function with given fields: featureID, resolve, core
func (_m *MockEObjectInternal) EGetFromID(featureID int, resolve bool, core bool) interface{} {
	ret := _m.Called(featureID, resolve, core)

	var r0 interface{}
	if rf, ok := ret.Get(0).(func(int, bool, bool) interface{}); ok {
		r0 = rf(featureID, resolve, core)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(interface{})
		}
	}

	return r0
}

// EGetResolve provides a mock function with given fields: _a0, _a1
func (_m *MockEObjectInternal) EGetResolve(_a0 EStructuralFeature, _a1 bool) interface{} {
	ret := _m.Called(_a0, _a1)

	var r0 interface{}
	if rf, ok := ret.Get(0).(func(EStructuralFeature, bool) interface{}); ok {
		r0 = rf(_a0, _a1)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(interface{})
		}
	}

	return r0
}

// EInverseAdd provides a mock function with given fields: otherEnd, featureID, notifications
func (_m *MockEObjectInternal) EInverseAdd(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	ret := _m.Called(otherEnd, featureID, notifications)

	var r0 ENotificationChain
	if rf, ok := ret.Get(0).(func(EObject, int, ENotificationChain) ENotificationChain); ok {
		r0 = rf(otherEnd, featureID, notifications)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotificationChain)
		}
	}

	return r0
}

// EInverseRemove provides a mock function with given fields: otherEnd, featureID, notifications
func (_m *MockEObjectInternal) EInverseRemove(otherEnd EObject, featureID int, notifications ENotificationChain) ENotificationChain {
	ret := _m.Called(otherEnd, featureID, notifications)

	var r0 ENotificationChain
	if rf, ok := ret.Get(0).(func(EObject, int, ENotificationChain) ENotificationChain); ok {
		r0 = rf(otherEnd, featureID, notifications)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotificationChain)
		}
	}

	return r0
}

// EInvoke provides a mock function with given fields: _a0, _a1
func (_m *MockEObjectInternal) EInvoke(_a0 EOperation, _a1 EList) interface{} {
	ret := _m.Called(_a0, _a1)

	var r0 interface{}
	if rf, ok := ret.Get(0).(func(EOperation, EList) interface{}); ok {
		r0 = rf(_a0, _a1)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(interface{})
		}
	}

	return r0
}

// EInvokeFromID provides a mock function with given fields: operationID, arguments
func (_m *MockEObjectInternal) EInvokeFromID(operationID int, arguments EList) interface{} {
	ret := _m.Called(operationID, arguments)

	var r0 interface{}
	if rf, ok := ret.Get(0).(func(int, EList) interface{}); ok {
		r0 = rf(operationID, arguments)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(interface{})
		}
	}

	return r0
}

// EIsProxy provides a mock function with given fields:
func (_m *MockEObjectInternal) EIsProxy() bool {
	ret := _m.Called()

	var r0 bool
	if rf, ok := ret.Get(0).(func() bool); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// EIsSet provides a mock function with given fields: _a0
func (_m *MockEObjectInternal) EIsSet(_a0 EStructuralFeature) bool {
	ret := _m.Called(_a0)

	var r0 bool
	if rf, ok := ret.Get(0).(func(EStructuralFeature) bool); ok {
		r0 = rf(_a0)
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// EIsSetFromID provides a mock function with given fields: featureID
func (_m *MockEObjectInternal) EIsSetFromID(featureID int) bool {
	ret := _m.Called(featureID)

	var r0 bool
	if rf, ok := ret.Get(0).(func(int) bool); ok {
		r0 = rf(featureID)
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// ENotificationRequired provides a mock function with given fields:
func (_m *MockEObjectInternal) ENotificationRequired() bool {
	ret := _m.Called()

	var r0 bool
	if rf, ok := ret.Get(0).(func() bool); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// ENotify provides a mock function with given fields: _a0
func (_m *MockEObjectInternal) ENotify(_a0 ENotification) {
	_m.Called(_a0)
}

// EObjectForFragmentSegment provides a mock function with given fields: _a0
func (_m *MockEObjectInternal) EObjectForFragmentSegment(_a0 string) EObject {
	ret := _m.Called(_a0)

	var r0 EObject
	if rf, ok := ret.Get(0).(func(string) EObject); ok {
		r0 = rf(_a0)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EObject)
		}
	}

	return r0
}

// EProxyURI provides a mock function with given fields:
func (_m *MockEObjectInternal) EProxyURI() *url.URL {
	ret := _m.Called()

	var r0 *url.URL
	if rf, ok := ret.Get(0).(func() *url.URL); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(*url.URL)
		}
	}

	return r0
}

// EResolveProxy provides a mock function with given fields: proxy
func (_m *MockEObjectInternal) EResolveProxy(proxy EObject) EObject {
	ret := _m.Called(proxy)

	var r0 EObject
	if rf, ok := ret.Get(0).(func(EObject) EObject); ok {
		r0 = rf(proxy)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EObject)
		}
	}

	return r0
}

// EResource provides a mock function with given fields:
func (_m *MockEObjectInternal) EResource() EResource {
	ret := _m.Called()

	var r0 EResource
	if rf, ok := ret.Get(0).(func() EResource); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EResource)
		}
	}

	return r0
}

// ESet provides a mock function with given fields: _a0, _a1
func (_m *MockEObjectInternal) ESet(_a0 EStructuralFeature, _a1 interface{}) {
	_m.Called(_a0, _a1)
}

// ESetDeliver provides a mock function with given fields: _a0
func (_m *MockEObjectInternal) ESetDeliver(_a0 bool) {
	_m.Called(_a0)
}

// ESetFromID provides a mock function with given fields: featureID, newValue
func (_m *MockEObjectInternal) ESetFromID(featureID int, newValue interface{}) {
	_m.Called(featureID, newValue)
}

// ESetProxyURI provides a mock function with given fields: uri
func (_m *MockEObjectInternal) ESetProxyURI(uri *url.URL) {
	_m.Called(uri)
}

// ESetResource provides a mock function with given fields: resource, notifications
func (_m *MockEObjectInternal) ESetResource(resource EResource, notifications ENotificationChain) ENotificationChain {
	ret := _m.Called(resource, notifications)

	var r0 ENotificationChain
	if rf, ok := ret.Get(0).(func(EResource, ENotificationChain) ENotificationChain); ok {
		r0 = rf(resource, notifications)
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotificationChain)
		}
	}

	return r0
}

// EStaticClass provides a mock function with given fields:
func (_m *MockEObjectInternal) EStaticClass() EClass {
	ret := _m.Called()

	var r0 EClass
	if rf, ok := ret.Get(0).(func() EClass); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(EClass)
		}
	}

	return r0
}

// EURIFragmentSegment provides a mock function with given fields: _a0, _a1
func (_m *MockEObjectInternal) EURIFragmentSegment(_a0 EStructuralFeature, _a1 EObject) string {
	ret := _m.Called(_a0, _a1)

	var r0 string
	if rf, ok := ret.Get(0).(func(EStructuralFeature, EObject) string); ok {
		r0 = rf(_a0, _a1)
	} else {
		r0 = ret.Get(0).(string)
	}

	return r0
}

// EUnset provides a mock function with given fields: _a0
func (_m *MockEObjectInternal) EUnset(_a0 EStructuralFeature) {
	_m.Called(_a0)
}

// EUnsetFromID provides a mock function with given fields: featureID
func (_m *MockEObjectInternal) EUnsetFromID(featureID int) {
	_m.Called(featureID)
}
