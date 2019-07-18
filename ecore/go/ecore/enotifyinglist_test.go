package ecore

import (
	"reflect"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
)

type eNotifyingListTest struct {
	*ENotifyingListImpl
	mockNotifier *MockENotifier
	mockFeature  *MockEStructuralFeature
	mockAdapter  *MockEAdapter
}

func newNotifyingListTestFn(factory func() *ENotifyingListImpl) *eNotifyingListTest {
	l := new(eNotifyingListTest)
	l.ENotifyingListImpl = factory()
	l.mockNotifier = new(MockENotifier)
	l.mockFeature = new(MockEStructuralFeature)
	l.mockAdapter = new(MockEAdapter)
	l.interfaces = l
	l.mockNotifier.On("EDeliver").Return(true)
	l.mockNotifier.On("EAdapters").Return(NewImmutableEList([]interface{}{l.mockAdapter}))
	return l
}

func newNotifyingListTest() *eNotifyingListTest {
	return newNotifyingListTestFn(NewENotifyingListImpl)
}

func newNotifyingListTestFromData(data []interface{}) *eNotifyingListTest {
	return newNotifyingListTestFn(func() *ENotifyingListImpl { return newENotifyingListImplFromData(data) })
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
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 3 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 0
	}))
	l.Add(3)

	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 4 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 1
	}))
	l.Add(4)
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{3, 4}, l.ToArray())

}

func TestNotifyingListAddAll(t *testing.T) {
	l := newNotifyingListTest()
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			reflect.DeepEqual(n.GetNewValue(), []interface{}{2, 3}) &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD_MANY &&
			n.GetPosition() == 0
	}))
	l.AddAll(NewImmutableEList([]interface{}{2, 3}))
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{2, 3}, l.ToArray())

	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 4 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 2
	}))
	l.AddAll(NewImmutableEList([]interface{}{4}))
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{2, 3, 4}, l.ToArray())
}

func TestNotifyingListInsert(t *testing.T) {
	l := newNotifyingListTest()
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 1 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 0
	}))
	l.Insert(0, 1)
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{1}, l.ToArray())

	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 2 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 0
	}))
	l.Insert(0, 2)
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{2, 1}, l.ToArray())

	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 3 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 1
	}))
	l.Insert(1, 3)
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{2, 3, 1}, l.ToArray())
}

func TestNotifyingListInsertAll(t *testing.T) {
	l := newNotifyingListTest()
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			reflect.DeepEqual(n.GetNewValue(), []interface{}{1, 2, 3}) &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD_MANY &&
			n.GetPosition() == 0
	}))
	l.InsertAll(0, NewImmutableEList([]interface{}{1, 2, 3}))
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{1, 2, 3}, l.ToArray())

	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			reflect.DeepEqual(n.GetNewValue(), []interface{}{4, 5}) &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD_MANY &&
			n.GetPosition() == 1
	}))
	l.InsertAll(1, NewImmutableEList([]interface{}{4, 5}))
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{1, 4, 5, 2, 3}, l.ToArray())
}

func TestNotifyingListSet(t *testing.T) {
	l := newNotifyingListTestFromData([]interface{}{1, 2})
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 3 &&
			n.GetOldValue() == 2 &&
			n.GetEventType() == SET &&
			n.GetPosition() == 1
	}))
	l.Set(1, 3)
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{1, 3}, l.ToArray())
}

func TestNotifyingListRemoveAt(t *testing.T) {
	l := newNotifyingListTestFromData([]interface{}{1, 2, 3})
	l.mockNotifier.On("ENotify", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == nil &&
			n.GetOldValue() == 2 &&
			n.GetEventType() == REMOVE &&
			n.GetPosition() == 1
	}))
	l.RemoveAt(1)
	l.assertExpectations(t)
	assert.Equal(t, []interface{}{1, 3}, l.ToArray())
}

func TestNotifyingListAddWithNotification(t *testing.T) {
	l := newNotifyingListTest()
	mockChain := new(MockENotificationChain)
	mockChain.On("Add", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 1 &&
			n.GetOldValue() == nil &&
			n.GetEventType() == ADD &&
			n.GetPosition() == 0
	})).Return(true)
	l.AddWithNotification(1, mockChain)
	l.assertExpectations(t)
	mockChain.AssertExpectations(t)
}

func TestNotifyingListRemoveWithNotification(t *testing.T) {
	l := newNotifyingListTestFromData([]interface{}{1})
	mockChain := new(MockENotificationChain)
	mockChain.On("Add", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == nil &&
			n.GetOldValue() == 1 &&
			n.GetEventType() == REMOVE &&
			n.GetPosition() == 0
	})).Return(true)
	l.RemoveWithNotification(1, mockChain)
	l.assertExpectations(t)
	mockChain.AssertExpectations(t)
}

func TestNotifyingListSetWithNotification(t *testing.T) {
	l := newNotifyingListTestFromData([]interface{}{1})
	mockChain := new(MockENotificationChain)
	mockChain.On("Add", mock.MatchedBy(func(n ENotification) bool {
		return n.GetNotifier() == l.mockNotifier &&
			n.GetFeature() == l.mockFeature &&
			n.GetNewValue() == 2 &&
			n.GetOldValue() == 1 &&
			n.GetEventType() == SET &&
			n.GetPosition() == 0
	})).Return(true)
	l.SetWithNotification(0, 2, mockChain)
	l.assertExpectations(t)
	mockChain.AssertExpectations(t)
}
