package ecore

type Adapter struct {
	target_ ENotifier
}

func NewAdapter() *Adapter {
	return &Adapter{target_: nil}
}

func (adapter *Adapter) GetTarget() ENotifier {
	return adapter.target_
}

func (adapter *Adapter) SetTarget(notifier ENotifier) {
	adapter.target_ = notifier
}
