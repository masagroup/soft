package ecore

// eClassifierExt is the extesnion of the model object 'EClassifier'
type eClassifierExt struct {
    *eClassifierImpl
}

func newEClassifierExt() *eClassifierExt {
	eClassifier := new(eClassifierExt)
	eClassifier.eClassifierImpl = newEClassifierImpl()
	eClassifier.internal = eClassifier
	return eClassifier
}
