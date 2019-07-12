package ecore

type eObjectEList struct {
	*ENotifyingListImpl
	owner            EObjectInternal
	featureID        int
	inverseFeatureID int
	containment      bool
	inverse          bool
	opposite         bool
	proxies          bool
	unset            bool
}

func NewEObjectEList(owner EObjectInternal, featureID int, inverseFeatureID int, containment, inverse, opposite, proxies, unset bool) *eObjectEList {
	list := &eObjectEList{
		owner:              owner,
		featureID:          featureID,
		inverseFeatureID:   inverseFeatureID,
		ENotifyingListImpl: NewENotifyingListImpl(),
		containment:        containment,
		inverse:            inverse,
		opposite:           opposite,
		proxies:            proxies,
		unset:              unset,
	}
	return list
}

func (arr *eObjectEList) Add(elem interface{}) bool {
	ret := arr.ENotifyingListImpl.Add(elem)
	if arr.inverse {
		var notifications ENotificationChain
		if arr.opposite {
			notifications = arr.owner.EInverseAdd(elem.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseAdd(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEList) AddAll(list EList) bool {
	ret := arr.ENotifyingListImpl.AddAll(list)
	if arr.inverse {
		var notifications ENotificationChain
		notifications = NewNotificationChain()
		if arr.opposite {
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

func (arr *eObjectEList) Insert(index int, elem interface{}) bool {
	ret := arr.ENotifyingListImpl.Insert(index, elem)
	if arr.inverse {
		var notifications ENotificationChain
		if arr.opposite {
			notifications = arr.owner.EInverseAdd(elem.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseAdd(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEList) InsertAll(index int, list EList) bool {
	ret := arr.ENotifyingListImpl.InsertAll(index, list)
	if arr.inverse {
		var notifications ENotificationChain
		notifications = NewNotificationChain()
		if arr.opposite {
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

func (arr *eObjectEList) Remove(elem interface{}) bool {
	ret := arr.ENotifyingListImpl.Remove(elem)
	if arr.inverse {
		var notifications ENotificationChain
		if arr.opposite {
			notifications = arr.owner.EInverseRemove(elem.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseRemove(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEList) RemoveAt(index int) interface{} {
	ret := arr.ENotifyingListImpl.RemoveAt(index)
	if arr.inverse {
		var notifications ENotificationChain
		if arr.opposite {
			notifications = arr.owner.EInverseRemove(ret.(EObject), arr.inverseFeatureID, nil)
		} else {
			notifications = arr.owner.EInverseRemove(ret.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, nil)
		}
		notifications.Dispatch()
	}
	return ret
}

func (arr *eObjectEList) Clear() {
	old := arr
	arr.ENotifyingListImpl.Clear()
	if arr.inverse {
		var notifications ENotificationChain
		if arr.opposite {
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

func (arr *eObjectEList) Set(index int, elem interface{}) {
	old := arr.Get(index)
	arr.ENotifyingListImpl.Set(index, elem)
	var notifications ENotificationChain
	if arr.opposite {
		notifications = arr.owner.EInverseRemove(old.(EObject), arr.inverseFeatureID, notifications)
		notifications = arr.owner.EInverseAdd(elem.(EObject), arr.inverseFeatureID, notifications)
	} else {
		notifications = arr.owner.EInverseRemove(old.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
		notifications = arr.owner.EInverseAdd(elem.(EObject), EOPPOSITE_FEATURE_BASE-arr.featureID, notifications)
	}
}
