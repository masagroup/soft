Reference ecore

// eReferenceExt is the extension of the model object 'EReference'
type eReferenceExt struct {
    *eReferenceImpl
}

func newEReferenceExt() *eReferenceExt {
	eReference := new(eReferenceExt)
	eReference.eReferenceImpl = newEReferenceImpl()
	return eReference
}
