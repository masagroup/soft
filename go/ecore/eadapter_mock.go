// Code generated by mockery v1.0.0. DO NOT EDIT.

package ecore

import mock "github.com/stretchr/testify/mock"

// MockEAdapter is an autogenerated mock type for the EAdapter type
type MockEAdapter struct {
	mock.Mock
}

// GetTarget provides a mock function with given fields:
func (_m *MockEAdapter) GetTarget() ENotifier {
	ret := _m.Called()

	var r0 ENotifier
	if rf, ok := ret.Get(0).(func() ENotifier); ok {
		r0 = rf()
	} else {
		if ret.Get(0) != nil {
			r0 = ret.Get(0).(ENotifier)
		}
	}

	return r0
}

// NotifyChanged provides a mock function with given fields: notification
func (_m *MockEAdapter) NotifyChanged(notification ENotification) {
	_m.Called(notification)
}

// SetTarget provides a mock function with given fields: _a0
func (_m *MockEAdapter) SetTarget(_a0 ENotifier) {
	_m.Called(_a0)
}
