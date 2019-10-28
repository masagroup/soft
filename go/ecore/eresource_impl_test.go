package ecore

import (
	"net/url"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
)

func TestResourceURI(t *testing.T) {
	u, err := url.Parse("https://example.com/foo%2fbar")
	assert.Nil(t, err)
	r := NewEResourceImpl()
	r.SetURI(u)
	assert.Equal(t, u, r.GetURI())
}

func TestResourceURINotifications(t *testing.T) {
	r := NewEResourceImpl()
	mockEAdapter := new(MockEAdapter)
	mockEAdapter.On("SetTarget", r).Once()
	mockEAdapter.On("NotifyChanged", mock.Anything).Once()
	r.EAdapters().Add(mockEAdapter)

	u, err := url.Parse("https://example.com/foo%2fbar")
	assert.Nil(t, err)
	r.SetURI(u)
	assert.Equal(t, u, r.GetURI())
	mockEAdapter.AssertExpectations(t)
}

func TestResourceContents(t *testing.T) {
	r := NewEResourceImpl()

	mockEObjectInternal := new(MockEObjectInternal)
	mockEObjectInternal.On("ESetResource", r, mock.Anything).Return(nil)
	r.GetContents().Add(mockEObjectInternal)

	mockEObjectInternal.On("ESetResource", nil, mock.Anything).Return(nil)
	r.GetContents().Remove(mockEObjectInternal)
}
