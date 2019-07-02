package ecore

type Notifier struct {
	eDeliver  bool
	eAdapters EList
}

func NewNotifier() *Notifier {
	notifier := new(Notifier)
	notifier.eDeliver = true
	notifier.eAdapters = NewEmptyArrayEList()
	return notifier
}

func (notifier *Notifier) EAdapters() EList {
	return notifier.eAdapters
}

func (notifier *Notifier) EDeliver() bool {
	return notifier.eDeliver
}

func (notifier *Notifier) ESetDeliver(value bool) {
	notifier.eDeliver = value
}

func (notifier *Notifier) ENotify(notification ENotification) {
	for it := notifier.eAdapters.Iterate(); it.Next(); {
		it.Value().(EAdapter).NotifyChanged(notification)
	}
}

func (notifier *Notifier) ENotificationRequired() bool {
	return notifier.eAdapters != nil && notifier.eAdapters.Size() > 0
}
