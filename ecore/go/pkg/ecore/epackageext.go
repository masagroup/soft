package ecore

// EPackageExt is the extension of the model object 'EFactory'
type EPackageExt struct {
    *ePackageImpl
}

func NewEPackageExt() *EPackageExt {
	ePackage := new(EPackageExt)
	ePackage.ePackageImpl = newEPackageImpl()
	return ePackage
}
