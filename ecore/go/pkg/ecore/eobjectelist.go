package ecore

type eObjectEList interface {
	EList
<<<<<<< HEAD
}

type eObjectEListInternal interface {
=======
>>>>>>> [Go] Add beginning of lists implementation
	containment() bool
	inverse() bool
	opposite() bool
	proxies() bool
	unset() bool
}
