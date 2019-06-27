package ecore

type ENotifyingList interface {
}

type ENotifyingListImpl struct {
	EList
	notifier *Notifier
}

func NewENotifyingListImpl() *ENotifyingListImpl {
	list := &ENotifyingListImpl{}
	list.notifier = NewNotifier()
	return list
}

// Add a new element to the array
func (arr *ENotifyingListImpl) Add(elem interface{}) bool {
	if arr.EList.Add(elem) {
		arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD, ECLASSIFIER__INSTANCE_CLASS, nil, elem, arr.Size()))
		return true
	}
	return false
}

// AddAll elements of an array in the current one
func (arr *ENotifyingListImpl) AddAll(list EList) bool {
	if arr.EList.AddAll(list) {
		if list.Size() == 1 {
			arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD, ECLASSIFIER__INSTANCE_CLASS, nil, list, arr.Size()))
		} else {
			arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD_MANY, ECLASSIFIER__INSTANCE_CLASS, nil, list, arr.Size()))
		}
		return true
	}
	return false
}
