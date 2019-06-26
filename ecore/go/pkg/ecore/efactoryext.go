package ecore

// eFactoryExt is the extension of the model object 'EFactory'
type eFactoryExt struct {
    *eFactoryImpl
}

func newEFactoryExt() *eFactoryExt {
	eFactory := new(eFactoryExt)
	eFactory.eFactoryImpl = newEFactoryImpl()
	return eFactory
}
