package ecore

const (
	NO_INDEX      = -1
	NO_FEATURE_ID = -1
)

type EventType int

const (
	CREATE EventType = iota
	SET
	UNSET
	ADD
	REMOVE
	ADD_MANY
	REMOVE_MANY
	MOVE
	REMOVING_ADAPTER
	RESOLVE
	EVENT_TYPE_COUNT
)

type ENotification interface {
	GetEventType() EventType

	GetNotifier() ENotifier

	GetFeature() EStructuralFeature

	GetFeatureId() int

	GetOldValue() interface{}

	GetNewValue() interface{}

	GetPosition() int

	Merge(ENotification) bool
}
