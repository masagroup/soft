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
	inv := arr.internal.(eObjectEListInternal).inverse()
	op := arr.internal.(eObjectEListInternal).opposite()
	if arr.ENotifyingListImpl.Add(elem) {
		if inv && !op {
		} else if inv && op {

		}
	}
	return false
}
