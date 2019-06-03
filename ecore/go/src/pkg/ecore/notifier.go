package ecore

type notifier struct {
	eDeliver_  bool
	eAdapters_ EList
}

func NewNotifier() *notifier {
	return &notifier{
		eDeliver_:  true,
		eAdapters_: NewArrayEList([]interface{}{}),
	}
}

func (notif *notifier) EAdapters() EList {
	return notif.eAdapters_
}

func (notif *notifier) EDeliver() bool {
	return notif.eDeliver_
}

func (notif *notifier) ESetDeliver(value bool) {
	notif.eDeliver_ = value
}

func (notif *notifier) ENotify(notification ENotification) {
	for it := notif.eAdapters_.Iterate(); it.Next(); {
		it.Value().(EAdapter).NotifyChanged(notification)
	}
}

func (notif *notifier) ENotificationRequired() bool {
	return notif.eAdapters_ != nil && notif.eAdapters_.Size() > 0
}
