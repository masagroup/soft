package ecore

import (
	"testing"

	mock "github.com/stretchr/testify/mock"
)

func TestENotifyingList(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 3 &&
				notif.GetEventType() == ADD
		})).Return()
		arr := NewENotifyingListImpl(mockOwner, -1)
		arr.Add(3)
	}
}
