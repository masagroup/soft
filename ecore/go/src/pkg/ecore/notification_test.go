package ecore

import (
	"fmt"
	"testing"
)

type customTestAdapter struct {
	Adapter
}

func (adapter *customTestAdapter) NotifyChanged(notification ENotification) {
	fmt.Print("Notification received!")
}

func TestNotification(t *testing.T) {
	testClassifier := EClassifierImpl{}
	notifier := NewNotifier()
	adapter := Adapter{target_: notifier}
	testClassifier.SetClassifierID(0)
	_ = adapter
}
