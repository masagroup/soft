package ecore

type ENotifyingList interface {
}

type ENotifyingListImpl struct {
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
	return list
}

// Add a new element to the array
func (arr *ENotifyingListImpl) Add(elem interface{}) bool {
	defer arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, ADD, arr.featureID, nil, elem, arr.Size()))
	return arr.arrayEList.Add(elem)
}

// AddAll elements of an array in the current one
func (arr *ENotifyingListImpl) AddAll(list EList) bool {
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
	oldArr := arr.ToArray()
	arr.arrayEList.Clear()
	if size == 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE, arr.featureID, oldArr[0], nil, NO_INDEX))
	} else if size > 1 {
		arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, REMOVE_MANY, arr.featureID, oldArr, nil, NO_INDEX))
	}
}

// Set an element of the array
func (arr *ENotifyingListImpl) Set(index int, elem interface{}) {
	old := arr.Get(index)
	arr.Set(index, elem)
	arr.owner.ENotify(NewNotificationByFeatureID(arr.owner, SET, arr.featureID, old, elem, index))
}
