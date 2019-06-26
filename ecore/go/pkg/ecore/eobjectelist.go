package ecore

type eObjectEList interface {
	EList
}

type eObjectEListInternal interface {
	containment() bool
	inverse() bool
	opposite() bool
	proxies() bool
	unset() bool
}
