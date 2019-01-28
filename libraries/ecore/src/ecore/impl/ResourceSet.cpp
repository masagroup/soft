#include "ecore/impl/ResourceSet.hpp"
#include "ecore/impl/Resource.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/impl/UriConverter.hpp"
#include "ecore/ENotifyingList.hpp"

using namespace ecore;
using namespace ecore::impl;


ResourceSet::ResourceSet()
    : resources_( [&]() { return initResources();  } )
    , uriConverter_( [&]() { return initUriConverter(); }  )
{
}

ResourceSet::~ResourceSet()
{
}

void ResourceSet::setThisPtr( const std::shared_ptr<ResourceSet>& thisPtr )
{
    thisPtr_ = thisPtr;
}

std::shared_ptr<ResourceSet> ResourceSet::getThisPtr() const
{
    return thisPtr_.lock();
}

std::shared_ptr<EList<std::shared_ptr<EResource>>> ResourceSet::getResources() const
{
    return resources_;
}

std::shared_ptr<EUriConverter> ecore::impl::ResourceSet::getUriConverter() const
{
    return uriConverter_;
}

void ResourceSet::setUriConverter( const std::shared_ptr<EUriConverter>& uriConverter )
{
    uriConverter_ = uriConverter;
}

std::shared_ptr<EList<std::shared_ptr<EResource>>> ResourceSet::initResources()
{
    class ResourcesEList : public AbstractENotifyingList<ENotifyingList<std::shared_ptr<EResource>>, std::shared_ptr<EResource>>
    {
    public:
        ResourcesEList( const std::weak_ptr<ResourceSet>& resourceSet )
            : resourceSet_( resourceSet )
        {

        }

        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return resourceSet_.lock();
        }

        virtual int getFeatureID() const
        {
            return 0;
        }

        virtual std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EResource>& object
                                                              , const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            auto resource = std::dynamic_pointer_cast<Resource>( object );
            return resource ? resource->basicSetResourceSet( resourceSet_.lock(), notifications ) : notifications;
        }

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EResource>& object
                                                                 , const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            auto resource = std::dynamic_pointer_cast<Resource>( object );
            return resource ? resource->basicSetResourceSet( nullptr, notifications ) : notifications;
        }

    private:
        std::weak_ptr<ResourceSet> resourceSet_;
    };

    return std::make_shared<ResourcesEList>( thisPtr_ );
}

std::shared_ptr<EUriConverter> ecore::impl::ResourceSet::initUriConverter()
{
    return std::make_shared<UriConverter>();
}
