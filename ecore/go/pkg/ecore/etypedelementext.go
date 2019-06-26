TypedElement ecore

// eTypedElementExt is the extension of the model object 'ETypedElement'
type eTypedElementExt struct {
    *eTypedElementImpl
}

func newETypedElementExt() *eTypedElementExt {
	eTypedElement := new(eTypedElementExt)
	eTypedElement.eTypedElementImpl = newETypedElementImpl()
	return eTypedElement
}
