package ecore

// eOperationExt is the extension of the model object 'EOperation'
type eOperationExt struct {
    *eOperationImpl
}

func newEOperationExt() *eOperationExt {
	eOperation := new(eOperationExt)
	eOperation.eOperationImpl = newEOperationImpl()
	eOperation.internal = eOperation
	return eOperation
}
