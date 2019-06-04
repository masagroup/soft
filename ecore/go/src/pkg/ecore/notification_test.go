package ecore

import (
	"fmt"
	"testing"
)

type customTestAdapter struct {
	Adapter
}

func (adapter *customTestAdapter) NotifyChanged(notification ENotification) {
	fmt.Println("Notification received!")
}

func TestNotification(t *testing.T) {
	testClassifier := NewEClassifierImpl()
	adapter := &customTestAdapter{Adapter: *NewAdapter()}
	testClassifier.Notifier.EAdapters().Add(adapter)
	testClassifier.SetClassifierID(0)
}
