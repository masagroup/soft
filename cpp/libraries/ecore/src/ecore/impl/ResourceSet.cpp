#include "ecore/impl/ResourceSet.hpp"
#include "ecore/impl/AbstractResource.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/impl/ResourceFactoryRegistry.hpp"
#include "ecore/impl/ResourceURIConverter.hpp"
#include "ecore/ENotifyingList.hpp"

using namespace ecore;
using namespace ecore::impl;


ResourceSet::ResourceSet()
    : resources_( [&]() { return initResources();  } )
    , uriConverter_( [&]() { return std::make_shared<ResourceURIConverter>(); } )
    , resourceFactoryRegistry_( [&]() { return std::make_shared<ResourceFactoryRegistry>(); } )
{
}

ResourceSet::~ResourceSet()
{
}

std::shared_ptr<EResource> ResourceSet::createResource(const URI& uri) const
{
    auto resourceFactory = resourceFactoryRegistry_->getFactory(uri);
    if (resourceFactory) {
        auto resource = resourceFactory->createResource(uri);
        resources_->add(resource);
        return resource;
    }
    return nullptr;
}

std::shared_ptr<EList<std::shared_ptr<EResource>>> ResourceSet::getResources() const
{
    return resources_;
}

std::shared_ptr<URIConverter> ecore::impl::ResourceSet::getURIConverter() const
{
    return uriConverter_;
}

void ResourceSet::setURIConverter( const std::shared_ptr<URIConverter>& uriConverter )
{
    uriConverter_ = uriConverter;
}

std::shared_ptr<EResourceFactoryRegistry> ResourceSet::gerResourceFactoryRegistry() const
{
    return resourceFactoryRegistry_;
}

void ResourceSet::setResourceFactoryRegistry( const std::shared_ptr<EResourceFactoryRegistry>& resourceFactoryRegistry )
{
    resourceFactoryRegistry_ = resourceFactoryRegistry;
}

std::shared_ptr<EList<std::shared_ptr<EResource>>> ResourceSet::initResources()
{
    class ResourcesEList : public AbstractENotifyingList<ENotifyingList<std::shared_ptr<EResource>>, std::shared_ptr<EResource>>
    {
    public:
        ResourcesEList( ResourceSet& resourceSet )
            : resourceSet_( resourceSet )
        {

        }

        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return resourceSet_.getThisPtr();
        }

        virtual int getFeatureID() const
        {
            return 0;
        }

        virtual std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EResource>& object
                                                              , const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            auto resource = std::dynamic_pointer_cast<AbstractResource>( object );
            return resource ? resource->basicSetResourceSet( resourceSet_.getThisPtr(), notifications ) : notifications;
        }

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EResource>& object
                                                                 , const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            auto resource = std::dynamic_pointer_cast<AbstractResource>( object );
            return resource ? resource->basicSetResourceSet( nullptr, notifications ) : notifications;
        }

    private:
        ResourceSet& resourceSet_;
    };

    return std::make_shared<ResourcesEList>( *this );
}
