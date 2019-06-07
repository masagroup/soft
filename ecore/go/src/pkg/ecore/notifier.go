package ecore

type Notifier struct {
	eDeliver_  bool
	eAdapters_ EList
}

func NewNotifier() *Notifier {
	return &Notifier{
		eDeliver_:  true,
		eAdapters_: NewArrayEList([]interface{}{}),
	}
}

func (notif *Notifier) EAdapters() EList {
	return notif.eAdapters_
}

func (notif *Notifier) EDeliver() bool {
	return notif.eDeliver_
}

func (notif *Notifier) ESetDeliver(value bool) {
	notif.eDeliver_ = value
}

func (notif *Notifier) ENotify(notification ENotification) {
	for it := notif.eAdapters_.Iterate(); it.Next(); {
		it.Value().(EAdapter).NotifyChanged(notification)
	}
}

func (notif *Notifier) ENotificationRequired() bool {
	return notif.eAdapters_ != nil && notif.eAdapters_.Size() > 0
}
