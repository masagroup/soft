package ecore

type eObjectEListImpl struct {
	*ENotifyingListImpl
	owner            EObjectInternal
	featureID        int
	inverseFeatureID int
	internal         interface{}
}

func NewEObjectEList(owner EObjectInternal, featureID int, inverseFeatureID int) *eObjectEListImpl {
	list := &eObjectEListImpl{
		owner:              owner,
		featureID:          featureID,
		inverseFeatureID:   inverseFeatureID,
		ENotifyingListImpl: NewENotifyingListImpl(owner, featureID),
	}
	return list
}

func (arr *eObjectEListImpl) Add(elem interface{}) bool {
	ret := arr.ENotifyingListImpl.Add(elem)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		if arr.internal.(eObjectEListInternal).opposite() {
			notifications = arr.owner.EInverseAdd(elem.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseAdd(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) AddAll(list EList) bool {
	ret := arr.ENotifyingListImpl.AddAll(list)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		notifications = NewNotificationChain()
		if arr.internal.(eObjectEListInternal).opposite() {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), arr.inverseFeatureID, notifications)
			}
		} else {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
			}
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) Insert(index int, elem interface{}) bool {
	ret := arr.ENotifyingListImpl.Insert(index, elem)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		if arr.internal.(eObjectEListInternal).opposite() {
			notifications = arr.owner.EInverseAdd(elem.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseAdd(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) InsertAll(index int, list EList) bool {
	ret := arr.ENotifyingListImpl.InsertAll(index, list)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		notifications = NewNotificationChain()
		if arr.internal.(eObjectEListInternal).opposite() {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), arr.inverseFeatureID, notifications)
			}
		} else {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
			}
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) AddAll(list EList) bool {
	ret := arr.ENotifyingListImpl.AddAll(list)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		notifications = NewNotificationChain()
		if arr.internal.(eObjectEListInternal).opposite() {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), arr.inverseFeatureID, notifications)
			}
		} else {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
			}
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) Insert(index int, elem interface{}) bool {
	ret := arr.ENotifyingListImpl.Insert(index, elem)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		if arr.internal.(eObjectEListInternal).opposite() {
			notifications = arr.owner.EInverseAdd(elem.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseAdd(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) InsertAll(index int, list EList) bool {
	ret := arr.ENotifyingListImpl.InsertAll(index, list)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		notifications = NewNotificationChain()
		if arr.internal.(eObjectEListInternal).opposite() {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), arr.inverseFeatureID, notifications)
			}
		} else {
			for it := list.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(it.Value().(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
			}
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) Remove(elem interface{}) bool {
	ret := arr.ENotifyingListImpl.Remove(elem)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		if arr.internal.(eObjectEListInternal).opposite() {
			notifications = arr.owner.EInverseRemove(elem.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseRemove(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) RemoveAt(index int) interface{} {
	ret := arr.ENotifyingListImpl.RemoveAt(index)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		if arr.internal.(eObjectEListInternal).opposite() {
			notifications = arr.owner.EInverseRemove(ret.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseRemove(ret.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) Clear() {
	old := arr
	arr.ENotifyingListImpl.Clear()
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		if arr.internal.(eObjectEListInternal).opposite() {
			for it := old.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseRemove(it.Value().(EObject), arr.inverseFeatureID, notifications)
			}
		} else {
			for it := old.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseRemove(it.Value().(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
			}
		}
		notifications.Dispatch()
	}
}

func (arr *eObjectEListImpl) Set(index int, elem interface{}) {
	old := arr.Get(index)
	arr.ENotifyingListImpl.Set(index, elem)
	var notifications ENotificationChain
	if arr.internal.(eObjectEListInternal).opposite() {
		notifications = arr.owner.EInverseRemove(old.(EObject), arr.inverseFeatureID, notifications)
		notifications = arr.owner.EInverseAdd(elem.(EObject), arr.inverseFeatureID, notifications)
	} else {
		notifications = arr.owner.EInverseRemove(old.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
		notifications = arr.owner.EInverseAdd(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
	}
}
