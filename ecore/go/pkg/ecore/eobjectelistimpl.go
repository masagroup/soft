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
