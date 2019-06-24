package ecore

// ENotificationChain is an accumulator of notifications.
// As notifications are produced,they are accumulated in a chain,
// and possibly even merged, before finally being dispatched to the notifier.
type ENotificationChain interface {

	// Add Adds a notification to the chain.
	Add(ENotification) bool

	// Dispatch Dispatches each notification to the appropriate notifier via notifier.ENotify method
	Dispatch()
}
