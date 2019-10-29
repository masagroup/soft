package ecore

type EDiagnostic interface {
	GetMessage() string
	GetLocation() string
	GetLine() int
	GetColumn() int
}
