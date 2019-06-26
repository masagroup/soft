package ecore

type eObjectEListImpl struct {
	*arrayEList
	internal interface{}
}

func NewEObjectEList() *eObjectEListImpl {
	factory := new(eObjectEListImpl)
	factory.arrayEList = NewArrayEList([]interface{}{})
	factory.internal = factory
	return factory
}

func (arr *eObjectEListImpl) resolve() bool {
	return arr.internal.(eObjectEListInternal).containment()
}
