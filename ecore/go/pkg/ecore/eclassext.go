package ecore

// eClassExt is the extension of the model object 'EClass'
type eClassExt struct {
    *eClassImpl
}

func newEClassExt() *eClassExt {
	eClass := new(eClassExt)
	eClass.eClassImpl = newEClassImpl()
	return eClass
}
