package ecore

// eModelElementExt is the extension of the model object 'EFactory'
type eModelElementExt struct {
    *eModelElementImpl
}

func newEModelElementExt() *eModelElementExt {
	eElement := new(eModelElementExt)
	eElement.eModelElementImpl = newEModelElementImpl()
	return eElement
}
