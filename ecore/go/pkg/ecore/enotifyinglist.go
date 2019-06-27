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

// Insert an element in the array
func (arr *ENotifyingListImpl) Insert(index int, elem interface{}) bool {
	if arr.EList.Insert(index, elem) {
		arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD, ECLASSIFIER__INSTANCE_CLASS, nil, elem, index))
		return true
	}
	return false
}

// InsertAll element of an array at a given position
func (arr *ENotifyingListImpl) InsertAll(index int, list EList) bool {
	if arr.EList.InsertAll(index, list) {
		if list.Size() == 1 {
			arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD, ECLASSIFIER__INSTANCE_CLASS, nil, list, index))
		} else {
			arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD_MANY, ECLASSIFIER__INSTANCE_CLASS, nil, list, index))
		}
		return true
	}
	return false
}

// RemoveAt remove an element at a given position
func (arr *ENotifyingListImpl) RemoveAt(index int) bool {
	old := arr.Get(index)
	if arr.EList.RemoveAt(index) {
		arr.notifier.ENotify(NewNotificationByFeatureID(nil, REMOVE, ECLASSIFIER__INSTANCE_CLASS, old, nil, index))
		return true
	}
	return false
}

// Remove an element in an array
func (arr *ENotifyingListImpl) Remove(elem interface{}) bool {
	index := arr.IndexOf(elem)
	old := arr.Get(index)
	if arr.EList.Remove(elem) {
		arr.notifier.ENotify(NewNotificationByFeatureID(nil, REMOVE, ECLASSIFIER__INSTANCE_CLASS, old, nil, index))
		return true
	}
	return false
}
