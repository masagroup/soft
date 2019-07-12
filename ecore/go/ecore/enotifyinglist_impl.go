package ecore

type eNotifyingListInternal interface {
	ENotifyingList

	inverseAdd( object interface{}, chain ENotificationChain ) ENotificationChain

	inverseRemove( object interface{}, chain ENotificationChain ) ENotificationChain
}

// ENotifyingListImpl ...
type ENotifyingListImpl struct {
	*arrayEList
}

// NewENotifyingListImpl ...
func NewENotifyingListImpl(owner EObject, featureID int) *ENotifyingListImpl {
	l := new(ENotifyingListImpl)
	l.arrayEList = NewUniqueArrayEList([]interface{}{})
	l.internal = l
	return l
}

func (list *ENotifyingListImpl) GetNotifier() ENotifier {
	return nil
}
	
func (list *ENotifyingListImpl) GetFeature() EStructuralFeature {
	return nil
}
	
func (list *ENotifyingListImpl) GetFeatureID() int {
	return -1
}

type notifyingListNotification struct {
	*abstractNotification
	list ENotifyingList
}

func (notif *notifyingListNotification) GetNotifier() ENotifier {
	return notif.list.GetNotifier()
}

func (notif *notifyingListNotification) GetFeature() EStructuralFeature {
	return notif.list.GetFeature()
}

func (notif *notifyingListNotification) GetFeatureID() int {
	return notif.list.GetFeatureID()
}

func (list *ENotifyingListImpl) createNotification( eventType EventType , oldValue interface{}, newValue interface{}, position int) ENotification {
	n := new( notifyingListNotification )
	n.abstractNotification = NewAbstractNotification( eventType , oldValue , newValue , position )
	n.internal = n
	n.list = list
	return n
}

func (list *ENotifyingListImpl) isNotificationRequired() bool {
	notifier := list.GetNotifier();
	if ( notifier != nil ) {
		return notifier.EDeliver() && !notifier.EAdapters().Empty()
	}
	return false
}

func (list *ENotifyingListImpl) createAndAddNotification( ns ENotificationChain, eventType EventType , oldValue interface{}, newValue interface{}, position int ) ENotificationChain {
	notifications := ns
	if list.isNotificationRequired() {
		notification := list.createNotification( eventType, oldValue, newValue, position )
        if( notifications != nil ) {
			notifications.Add( notification )
		} else {
			notifications = notification.(ENotificationChain)
		}
    }
	return notifications
}

func (list *ENotifyingListImpl) createAndDispatchNotification( notifications ENotificationChain, eventType EventType , oldValue interface{}, newValue interface{}, position int ) {
	list.createAndDispatchNotificationFn( notifications, func () ENotification {
		return list.createNotification( eventType , oldValue , newValue , position )
	} )
}

type notificationFactory func() ENotification

func (list *ENotifyingListImpl) createAndDispatchNotificationFn( notifications ENotificationChain, createNotification notificationFactory ) {
	if list.isNotificationRequired() {
		notification := createNotification()
		if notifications != nil {
			notifications.Add( notification )
			notifications.Dispatch()
		}
		notifier := list.GetNotifier()
		if notifier != nil  {
			notifier.ENotify( notification );
		}
	} else {
		if notifications != nil {
			notifications.Dispatch()
		}
	}
}

func (list *ENotifyingListImpl) inverseAdd( object interface{}, notifications ENotificationChain ) ENotificationChain {
	return notifications
}

func (list *ENotifyingListImpl) inverseRemove( object interface{}, notifications ENotificationChain ) ENotificationChain {
	return notifications
}

func (list *ENotifyingListImpl) AddWithNotification( object interface{} , notifications ENotificationChain ) ENotificationChain {
	index := list.Size();
	list.arrayEList.doAdd( object )
    return list.createAndAddNotification( notifications, ADD, nil, object, index )
}

func (list *ENotifyingListImpl) RemoveWithNotification( object interface{} , notifications ENotificationChain ) ENotificationChain {
	index := list.IndexOf( object );
	if( index != -1 ) {
		oldObject := list.arrayEList.Remove( index )
		return list.createAndAddNotification( notifications, REMOVE, oldObject, nil, index )
	}
	return notifications
}
	
func (list *ENotifyingListImpl) SetWithNotification( index int, object interface{} , notifications ENotificationChain ) ENotificationChain {
	oldObject := list.arrayEList.doSet( index, object )
    return list.createAndAddNotification( notifications, SET, oldObject, object, index )
}

func (list *ENotifyingListImpl) doAdd(object interface{}) {
	index := list.Size()
	list.arrayEList.doAdd( object )
	notifications := list.internal.(eNotifyingListInternal).inverseAdd( object, nil )
	list.createAndDispatchNotification( notifications, ADD, nil, object, index )
}

func (list *ENotifyingListImpl) doAddAll(l EList) bool {
	return list.doInsertAll( list.Size(), l )
}

func (list *ENotifyingListImpl) doInsert(index int, object interface{}) {
	list.arrayEList.doInsert(index, object)
	notifications := list.internal.(eNotifyingListInternal).inverseAdd( object, nil )
	list.createAndDispatchNotification( notifications, ADD, nil, object, index )
}

func (list *ENotifyingListImpl) doInsertAll(index int, l EList) bool {
	if( l.Empty() ) {
		return false
	}
		
	result := list.arrayEList.doInsertAll( index, l )
	var notifications ENotificationChain = NewNotificationChain()
	for it := l.Iterate(); it.Next(); {
		notifications = list.internal.(eNotifyingListInternal).inverseAdd( it.Value(), notifications )
	}
	list.createAndDispatchNotificationFn( notifications, func () ENotification {
		if l.Size() == 1 {
			return list.createNotification( ADD, nil, l.Get( 0 ), index )
		} else {
			return list.createNotification( ADD_MANY, nil, l.ToArray() , index );
		}
	} )
	return result
}

func (list *ENotifyingListImpl) doSet(index int, newObject interface{}) interface{} {
	oldObject := list.arrayEList.doSet( index, newObject )
	if newObject != oldObject {
		var notifications ENotificationChain
		notifications = list.internal.(eNotifyingListInternal).inverseRemove( oldObject, notifications );
		notifications = list.internal.(eNotifyingListInternal).inverseAdd( newObject, notifications );
		list.createAndDispatchNotification( notifications, SET, oldObject, newObject, index );
	}
	return oldObject;
}
