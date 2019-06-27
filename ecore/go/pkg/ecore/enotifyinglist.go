package ecore

type ENotifyingList interface {
}

type ENotifyingListImpl struct {
<<<<<<< HEAD
	*arrayEList
	owner     EObject
	featureID int
}

func NewENotifyingListImpl(owner EObject, featureID int) *ENotifyingListImpl {
	list := &ENotifyingListImpl{
		owner:      owner,
		featureID:  featureID,
		arrayEList: NewUniqueArrayEList([]interface{}{}),
	}
=======
	EList
	notifier *Notifier
}

func NewENotifyingListImpl() *ENotifyingListImpl {
	list := &ENotifyingListImpl{}
	list.notifier = NewNotifier()
>>>>>>> [Go] Begin to implement Notifying List
	return list
}

// Add a new element to the array
func (arr *ENotifyingListImpl) Add(elem interface{}) bool {
<<<<<<< HEAD
	defer arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, elem, arr.Size()))
	return arr.arrayEList.Add(elem)
=======
	if arr.EList.Add(elem) {
		arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD, ECLASSIFIER__INSTANCE_CLASS, nil, elem, arr.Size()))
		return true
	}
	return false
>>>>>>> [Go] Begin to implement Notifying List
}

// AddAll elements of an array in the current one
func (arr *ENotifyingListImpl) AddAll(list EList) bool {
<<<<<<< HEAD
	val := arr.arrayEList.AddAll(list)
	size := arr.Size()
	if size == 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, list.Get(0), arr.Size()))
	} else if size > 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD_MANY, arr.featureID, nil, list.ToArray(), arr.Size()))
	}
	return val
}

// Insert an element in the array
func (arr *ENotifyingListImpl) Insert(index int, elem interface{}) bool {
	defer arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, elem, index))
	return arr.arrayEList.Insert(index, elem)
}

// InsertAll element of an array at a given position
func (arr *ENotifyingListImpl) InsertAll(index int, list EList) bool {
	val := arr.arrayEList.InsertAll(index, list)
	size := list.Size()
	if size == 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, list.Get(0), index))
	} else if size > 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD_MANY, arr.featureID, nil, list.ToArray(), index))
	}
	return val
}

// RemoveAt remove an element at a given position
func (arr *ENotifyingListImpl) RemoveAt(index int) interface{} {
	old := arr.Get(index)
	elem := arr.arrayEList.RemoveAt(index)
	arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE, arr.featureID, old, nil, index))
	return elem
}

// Remove an element in an array
func (arr *ENotifyingListImpl) Remove(elem interface{}) bool {
	index := arr.IndexOf(elem)
	old := arr.Get(index)
	defer arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE, arr.featureID, old, nil, index))
	return arr.arrayEList.Remove(elem)
}

// Clear remove all elements of the array
func (arr *ENotifyingListImpl) Clear() {
	size := arr.Size()
	oldArr := arr
	arr.arrayEList.Clear()
	if size == 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE, arr.featureID, oldArr.Get(0), nil, NO_INDEX))
	} else if size > 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE_MANY, arr.featureID, oldArr.ToArray(), nil, NO_INDEX))
	}
}

// Set an element of the array
func (arr *ENotifyingListImpl) Set(index int, elem interface{}) {
	old := arr.Get(index)
	arr.Set(index, elem)
	arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, SET, arr.featureID, old, elem, index))
=======
	if arr.EList.AddAll(list) {
		if list.Size() == 1 {
			arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD, ECLASSIFIER__INSTANCE_CLASS, nil, list, arr.Size()))
		} else {
			arr.notifier.ENotify(NewNotificationByFeatureID(nil, ADD_MANY, ECLASSIFIER__INSTANCE_CLASS, nil, list, arr.Size()))
		}
		return true
	}
	return false
>>>>>>> [Go] Begin to implement Notifying List
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
