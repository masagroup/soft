#include "ecore/impl/Resource.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/ENotifyingList.hpp"

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

std::shared_ptr<EList<std::shared_ptr<EObject>>> Resource::initContents() const
{
    class ContentsEList : public AbstractENotifyingList< ENotifyingList< std::shared_ptr<EObject> >, std::shared_ptr<EObject> >
    {
    public:
        ContentsEList( const Resource& resource )
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

        virtual std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return notifications;
        }

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return notifications;
        }

    private:
        
        std::shared_ptr<Resource> getThisPtr() const
        {
            return resource_.getThisPtr();
        }

    private:
        const Resource& resource_;
    };


    return std::make_shared<ContentsEList>(*this);
}
