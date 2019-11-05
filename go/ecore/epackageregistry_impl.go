package ecore

type EPackageRegistryImpl struct {
	packages map[string]EPackage
	delegate EPackageRegistry
}

func NewEPackageRegistryImpl() *EPackageRegistryImpl {
	r := &EPackageRegistryImpl{
		packages: map[string]EPackage{},
	}
	return r
}

func NewEPackageRegistryImplWithDelegate(delegate EPackageRegistry) *EPackageRegistryImpl {
	r := &EPackageRegistryImpl{
		packages: map[string]EPackage{},
		delegate: delegate,
	}
	return r
}

func (r *EPackageRegistryImpl) RegisterPackage(pack EPackage) {
	r.packages[pack.GetNsURI()] = pack
}

func (r *EPackageRegistryImpl) UnregisterPackage(pack EPackage) {
	delete(r.packages, pack.GetNsURI())
}

func (r *EPackageRegistryImpl) GetPackage(nsURI string) EPackage {
	p, _ := r.packages[nsURI]
	if p != nil {
		return p
	} else {
		if r.delegate != nil {
			return r.delegate.GetPackage(nsURI)
		}
	}
	return nil
}

func (r *EPackageRegistryImpl) GetFactory(nsURI string) EFactory {
	p, _ := r.packages[nsURI]
	if p != nil {
		return p.GetEFactoryInstance()
	} else {
		if r.delegate != nil {
			return r.delegate.GetFactory(nsURI)
		}
	}
	return nil
}
