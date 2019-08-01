// Code generated by mockery v1.0.0. DO NOT EDIT.

package ecore

import mock "github.com/stretchr/testify/mock"

// MockENotifier is an autogenerated mock type for the ENotifier type
type MockENotifier struct {
	mock.Mock
}

// EAdapters provides a mock function with given fields:
func (_m *MockENotifier) EAdapters() EList {
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
func (_m *MockENotifier) EDeliver() bool {
	ret := _m.Called()

	var r0 bool
	if rf, ok := ret.Get(0).(func() bool); ok {
		r0 = rf()
	} else {
		r0 = ret.Get(0).(bool)
	}

	return r0
}

// ENotificationRequired provides a mock function with given fields:
func (_m *MockENotifier) ENotificationRequired() bool {
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
func (_m *MockENotifier) ENotify(_a0 ENotification) {
	_m.Called(_a0)
}

// ESetDeliver provides a mock function with given fields: _a0
func (_m *MockENotifier) ESetDeliver(_a0 bool) {
	_m.Called(_a0)
}
