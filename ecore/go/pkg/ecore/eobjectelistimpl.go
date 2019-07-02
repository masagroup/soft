package ecore

type eObjectEListImpl struct {
	*ENotifyingListImpl
	owner            EObject
	featureID        int
	inverseFeatureID int
	internal         interface{}
}

func NewEObjectEList(owner EObject, featureID int, inverseFeatureID int) *eObjectEListImpl {
	list := &eObjectEListImpl{
		owner:              owner,
		featureID:          featureID,
		inverseFeatureID:   inverseFeatureID,
		ENotifyingListImpl: NewENotifyingListImpl(owner, featureID),
	}
	return list
}

func (arr *eObjectEListImpl) resolve() bool {
	return arr.internal.(eObjectEListInternal).containment()
}
