package ecore

type eObjectEListImpl struct {
	arrayEList
}

func NewEObjectEList() *eObjectEListImpl {
	
}

func (arr *eObjectEListImpl) test() bool {
	return arr.containment()
}