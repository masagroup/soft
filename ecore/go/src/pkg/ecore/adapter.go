package ecore

type Adapter struct {
	target_ ENotifier
}

func (adapter *Adapter) GetTarget() ENotifier {
	return adapter.target_
}

func (adapter *Adapter) SetTarget(notifier ENotifier) {
	if adapter.target_ == notifier {
		return
	}
	adapters := adapter.target_.EAdapters()
	adapters.Remove(adapter)
	if adapters.Size() == 0 {

	}
	//
}
