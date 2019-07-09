// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

const (
	// NO_INDEX constant indicating that no position information is applicable..
	NO_INDEX = -1
	// NO_FEATURE_ID constant indicating that no feature ID information is applicable.
	NO_FEATURE_ID = -1
)

type EventType int

const (
	// CREATE An event type indicating that the notifier has been created.
	CREATE EventType = iota
	// SET An event type indicating that a feature of the notifier has been set.
	SET
	// UNSET An event type indicating that a feature of the notifier has been set.
	UNSET
	// ADD An event type indicating that a feature of the notifier has been unset.
	ADD
	// REMOVE An event type indicating that a feature of the notifier has been set.
	REMOVE
	// ADD_MANY An event type indicating that a several values have been added into a list-based feature of the notifier..
	ADD_MANY
	// REMOVE_MANY An event type indicating that a several values have been removed from a list-based feature of the notifier..
	REMOVE_MANY
	// MOVE An event type indicating that a value has been moved within a list-based feature of the notifier.
	MOVE
	// REMOVING_ADAPTER An event type indicating that an adapter is being removed from the notifier.
	REMOVING_ADAPTER
	// RESOLVE An event type indicating that a feature of the notifier has been resolved from a proxy.
	RESOLVE
	// EVENT_TYPE_COUNT User defined event types should start from this value.
	EVENT_TYPE_COUNT
)

// ENotification A description of a feature change that has occurred for some notifier.
type ENotification interface {

	// GetEventType Returns the type of change that has occurred.
	GetEventType() EventType

	// GetNotifier Returns the object affected by the change.
	GetNotifier() ENotifier

	// GetFeature Returns the object representing the feature of the notifier that has changed.
	GetFeature() EStructuralFeature

	// GetFeatureID Returns the numeric ID of the feature relative to the given class, or NO_FEATURE_ID when not applicable.
	GetFeatureID() int

	// GetOldValue Returns the value of the notifier's feature before the change occurred.
	// For a list-based feature, this represents a value, or a list of values, removed from the list.
	// For a move, this represents the old position of the moved value.
	GetOldValue() interface{}

	// GetNewValue Returns the value of the notifier's feature after the change occurred.
	// For a list-based feature, this represents a value, or a list of values, added to the list,
	// an array of int containing the original index of each value in the list of values removed from the list (except for the case of a clear),
	// the value moved within the list, or nill otherwise.
	GetNewValue() interface{}

	// GetPosition Returns the position within a list-based feature at which the change occurred.
	// It returns NO_INDEX when not applicable.
	GetPosition() int

	// Merge Returns whether the notification can be and has been merged with this one.
	Merge(ENotification) bool
}
