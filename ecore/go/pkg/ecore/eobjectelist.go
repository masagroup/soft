package ecore

type eObjectEList interface {
	EList
	containment() bool
	inverse() bool
	opposite() bool
	proxies() bool
	unset() bool
}
