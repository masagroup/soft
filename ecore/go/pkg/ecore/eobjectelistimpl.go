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
			notifications = arr.owner.EInverseAdd(arr.owner, arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseAdd(arr.owner, EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
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
			for it := arr.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(arr.owner, arr.inverseFeatureID, notifications)
			}
		} else {
			for it := arr.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(arr.owner, EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
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
			notifications = arr.owner.EInverseAdd(arr.owner, arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseAdd(arr.owner, EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
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
			for it := arr.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(arr.owner, arr.inverseFeatureID, notifications)
			}
		} else {
			for it := arr.Iterate(); it.Next(); {
				notifications = arr.owner.EInverseAdd(arr.owner, EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
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
			notifications = arr.owner.EInverseRemove(arr.owner, arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseRemove(arr.owner, EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEListImpl) RemoveAt(index int) interface{} {
	ret := arr.ENotifyingListImpl.Remove(index)
	if arr.internal.(eObjectEListInternal).inverse() {
		var notifications ENotificationChain
		if arr.internal.(eObjectEListInternal).opposite() {
			notifications = arr.owner.EInverseRemove(arr.owner, arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseRemove(arr.owner, EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}
