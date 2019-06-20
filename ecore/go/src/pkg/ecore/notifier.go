package ecore

type Notifier struct {
	eDeliver  bool
	eAdapters EList
}

func NewNotifier() *Notifier {
	return &Notifier{
		eDeliver:  true,
		eAdapters: NewArrayEList([]interface{}{}),
	}
}

func (notif *Notifier) EAdapters() EList {
	return notif.eAdapters
}

func (notif *Notifier) EDeliver() bool {
	return notif.eDeliver
}

func (notif *Notifier) ESetDeliver(value bool) {
	notif.eDeliver = value
}

func (notif *Notifier) ENotify(notification ENotification) {
	for it := notif.eAdapters.Iterate(); it.Next(); {
		it.Value().(EAdapter).NotifyChanged(notification)
	}
}

func (notif *Notifier) ENotificationRequired() bool {
	return notif.eAdapters != nil && notif.eAdapters.Size() > 0
}
