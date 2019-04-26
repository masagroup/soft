package ecore

type ENotificationChain interface {
	Add(ENotification) bool

	Dispatch()
}
