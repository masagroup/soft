package ecore

type EPackageRegistry interface {
	registerPackage(pack EPackage)
	unregisterPackage(pack EPackage)

	getPackage(nsURI string) EPackage
	getFactory(nsURI string) EFactory
}
