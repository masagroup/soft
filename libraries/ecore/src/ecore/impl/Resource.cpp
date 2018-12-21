#include "ecore/impl/Resource.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/ENotifyingList.hpp"
#include "ecore/EObject.hpp"

using namespace ecore;
using namespace ecore::impl;

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
