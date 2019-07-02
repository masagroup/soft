package ecore

// EFactoryExt is the extension of the model object 'EFactory'
type EFactoryExt struct {
    *eFactoryImpl
}

func NewEFactoryExt() *EFactoryExt {
	eFactory := new(EFactoryExt)
	eFactory.eFactoryImpl = newEFactoryImpl()
	eFactory.internal = eFactory
	return eFactory
}
