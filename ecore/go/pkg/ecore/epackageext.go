package ecore

// ePackageExt is the extension of the model object 'EPackage'
type ePackageExt struct {
    *ePackageImpl
}

func newEPackageExt() *ePackageExt {
	ePackage := new(ePackageExt)
	ePackage.ePackageImpl = newEPackageImpl()
	return ePackage
}
