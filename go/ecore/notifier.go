package ecore

type adapterList struct {
	*arrayEList
	notifier *Notifier
}

func newAdapterList(notifier *Notifier) *adapterList {
	l := new(adapterList)
	l.arrayEList = NewEmptyArrayEList()
	l.notifier = notifier
	l.interfaces = l
	return l
}

func (l *adapterList) didAdd(index int, elem interface{}) {
	elem.(EAdapter).SetTarget(l.notifier.interfaces.(ENotifier))
}

func (l *adapterList) didRemove(index int, elem interface{}) {
	elem.(EAdapter).SetTarget(nil)
}

type Notifier struct {
	interfaces interface{}
	eDeliver   bool
	eAdapters  EList
}

func NewNotifier() *Notifier {
	notifier := new(Notifier)
	notifier.interfaces = notifier
	notifier.eDeliver = true
	notifier.eAdapters = newAdapterList(notifier)
	return notifier
}

// SetInterfaces ...
func (o *Notifier) SetInterfaces(interfaces interface{}) {
	o.interfaces = interfaces
}

// GetInterfaces ...
func (o *Notifier) GetInterfaces() interface{} {
	return o.interfaces
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
	for it := notifier.eAdapters.Iterator(); it.HasNext(); {
		it.Next().(EAdapter).NotifyChanged(notification)
	}
}

func (notifier *Notifier) ENotificationRequired() bool {
	return notifier.eAdapters != nil && notifier.eAdapters.Size() > 0
}
