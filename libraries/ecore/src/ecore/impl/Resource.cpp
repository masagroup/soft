#include "ecore/impl/Resource.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/impl/AbstractNotification.hpp"
#include "ecore/ENotifyingList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/Stream.hpp"


using namespace ecore;
using namespace ecore::impl;


class Resource::Notification : public AbstractNotification
{
public:
    Notification( const std::weak_ptr<ENotifier>& notifier , EventType type,
                  int featureID,
                  const Any& oldValue,
                  const Any& newValue,
                  std::size_t position = NO_INDEX )
        : AbstractNotification( type, oldValue , newValue, position ) 
        , notifier_( notifier )
        , featureID_( featureID )
    {
    }

    virtual std::shared_ptr<ENotifier> getNotifier() const
    {
        return notifier_.lock();
    }

    virtual int getFeatureID() const
    {
        return featureID_;
    }

    virtual std::shared_ptr<EStructuralFeature> getFeature() const
    {
        return nullptr;
    }

private:
    std::weak_ptr<ENotifier> notifier_;
    int featureID_;
};

Resource::Resource()
    : eContents_( [ & ]()
{
    return initContents();
} )
{
}

Resource::~Resource()
{
}

void Resource::setThisPtr( const std::shared_ptr<Resource>& resource )
{
    thisPtr_ = resource;
}

std::shared_ptr<Resource> Resource::getThisPtr() const
{
    return thisPtr_.lock();
}

const Uri& Resource::getUri() const
{
    return uri_;
}

void Resource::setUri( const Uri& uri )
{
    Uri oldUri = uri_;
    uri_ = uri;
    if( eNotificationRequired() )
        eNotify( std::make_shared<Notification>( thisPtr_, Notification::SET, RESOURCE__URI, oldUri , uri_) );
}

std::shared_ptr<EList<std::shared_ptr<EObject>>> Resource::getContents() const
{
    return eContents_;
}

std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> Resource::getAllContents() const
{
    return std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>>();
}

void Resource::attached( const std::shared_ptr<EObject>& object )
{
}

void Resource::detached( const std::shared_ptr<EObject>& object )
{
}

std::shared_ptr<EList<std::shared_ptr<EObject>>> Resource::initContents()
{
    class ContentsEList : public AbstractENotifyingList< ENotifyingList< std::shared_ptr<EObject> >, std::shared_ptr<EObject> >
    {
    public:
        ContentsEList( Resource& resource )
            : resource_( resource )
        {

        }

        virtual ~ContentsEList()
        {

        }

        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return getThisPtr();
        }

        virtual int getFeatureID() const
        {
            return EResource::RESOURCE__CONTENTS;
        }

    protected:

        virtual std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<ENotificationChain>& n ) const
        {
            auto notifications = n;
            notifications = eObject->eSetResource( getThisPtr(), notifications );
            resource_.attached( eObject );
            return notifications;
        }

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<ENotificationChain>& n ) const
        {
            auto notifications = n;
            resource_.detached( eObject );
            notifications = eObject->eSetResource( nullptr, notifications );
            return notifications;
        }

    private:
        
        std::shared_ptr<Resource> getThisPtr() const
        {
            return resource_.getThisPtr();
        }

    private:
        Resource& resource_;
    };


    return std::make_shared<ContentsEList>(*this);
}
