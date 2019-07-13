package ecore

import (
	"reflect"
	"testing"

	"github.com/stretchr/testify/mock"
	"github.com/stretchr/testify/assert"
)

type eNotifyingListTest struct {
	*ENotifyingListImpl
	mockNotifier *MockENotifier
	mockFeature *MockEStructuralFeature
	mockAdapter *MockEAdapter
}

func newNotifyingListTest() *eNotifyingListTest {
	l := new(eNotifyingListTest)
	l.ENotifyingListImpl = NewENotifyingListImpl()
	l.mockNotifier = new(MockENotifier) 
	l.mockFeature = new(MockEStructuralFeature) 
	l.mockAdapter = new(MockEAdapter) 
	l.internal = l
	l.mockNotifier.On("EDeliver").Return( true )
	l.mockNotifier.On("EAdapters").Return( NewImmutableEList([]interface{}{l.mockAdapter}) )
	return l
}

func (list *eNotifyingListTest) GetNotifier() ENotifier {
	return list.mockNotifier
}
	
func (list *eNotifyingListTest) GetFeature() EStructuralFeature {
	return list.mockFeature
}
	
func (list *eNotifyingListTest) GetFeatureID() int {
	return list.mockFeature.GetFeatureID()
}

func (list *eNotifyingListTest) assertExpectations(t *testing.T) {
	list.mockNotifier.AssertExpectations(t)
	list.mockFeature.AssertExpectations(t)
	list.mockAdapter.AssertExpectations(t)
}


func TestNotifyingListAdd(t *testing.T) {
	l := newNotifyingListTest()
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetNewValue() == 3 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 0
	}))
	l.Add(3)

	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetNewValue() == 4 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 1
	}))
	l.Add(4)
	l.assertExpectations(t)
	assert.Equal( t , []interface{}{3,4} , l.ToArray() )
	
}

func TestNotifyingListAddAll(t *testing.T) {
	l := newNotifyingListTest()
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
		    reflect.DeepEqual(n.GetNewValue(), []interface{}{2, 3}) &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD_MANY &&
			n.GetPosition() == 0
	}))
	l.AddAll( NewImmutableEList([]interface{}{2,3}) )
	l.assertExpectations(t)
	assert.Equal( t , []interface{}{2,3} , l.ToArray() )
	
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
		    n.GetNewValue() == 4 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 2
	}))
	l.AddAll( NewImmutableEList([]interface{}{4}) )
	l.assertExpectations(t)
	assert.Equal( t , []interface{}{2,3,4} , l.ToArray() )	
}

func TestNotifyingListInsert(t *testing.T) {
	l := newNotifyingListTest()
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
		    n.GetNewValue() == 1 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 0
	}))
	l.Insert( 0 , 1 )
	l.assertExpectations(t)
	assert.Equal( t , []interface{}{1} , l.ToArray() )
	
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
		    n.GetNewValue() == 2 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 0
	}))
	l.Insert( 0 , 2 )
	l.assertExpectations(t)
	assert.Equal( t , []interface{}{2, 1} , l.ToArray() )

	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
		    n.GetNewValue() == 3 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 1
	}))
	l.Insert( 1 , 3 )
	l.assertExpectations(t)
	assert.Equal( t , []interface{}{2, 3, 1} , l.ToArray() )
}


func CreateTestNotifyingArray(mockOwner *MockEObjectInternal) *ENotifyingListImpl {
	mockOwner.On("ENotify", mock.Anything).Return()
	templateArr := NewENotifyingListImpl()
	templateArr.Add(10)
	return templateArr
}

func TestENotifyingList(t *testing.T) {
	mockOwner := &MockEObjectInternal{}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 3 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Add(3)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 1 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.AddAll(NewArrayEList([]interface{}{1}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), []interface{}{1, 2, 3}) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD_MANY
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.InsertAll(0, NewArrayEList([]interface{}{1, 2, 3}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == 3 &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Insert(0, 3)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), 1) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.InsertAll(0, NewArrayEList([]interface{}{1}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return reflect.DeepEqual(notif.GetNewValue(), []interface{}{1, 2, 3}) &&
				notif.GetOldValue() == nil &&
				notif.GetEventType() == ADD_MANY
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.InsertAll(0, NewArrayEList([]interface{}{1, 2, 3}))
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == nil &&
				notif.GetOldValue() == 10 &&
				notif.GetEventType() == REMOVE
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.RemoveAt(0)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == nil &&
				notif.GetOldValue() == 10 &&
				notif.GetEventType() == REMOVE
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Remove(10)
	}
	{
		mockOwner.On("ENotify", mock.MatchedBy(func(notif *notification) bool {
			return notif.GetNewValue() == nil &&
				notif.GetOldValue() == 10 &&
				notif.GetEventType() == REMOVE
		})).Return()
		arr := CreateTestNotifyingArray(mockOwner)
		arr.Clear()
	}
}
