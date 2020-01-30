#include "ecore/impl/AbstractResource.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/ECollectionView.hpp"
#include "ecore/ENotificationChain.hpp"
#include "ecore/ENotifyingList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EResourceSet.hpp"
#include "ecore/EcoreUtils.hpp"
#include "ecore/Stream.hpp"
#include "ecore/URIConverter.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"
#include "ecore/impl/AbstractNotification.hpp"
#include "ecore/impl/EObjectInternal.hpp"
#include "ecore/impl/ResourceURIConverter.hpp"
#include "ecore/impl/StringUtils.hpp"

#include <cctype>
#include <sstream>

using namespace ecore;
using namespace ecore::impl;

class AbstractResource::Notification : public AbstractNotification
{
public:
    Notification( const std::weak_ptr<ENotifier>& notifier,
                  EventType type,
                  int featureID,
                  const Any& oldValue,
                  const Any& newValue,
                  std::size_t position = NO_INDEX )
        : AbstractNotification( type, oldValue, newValue, position )
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

AbstractResource::AbstractResource()
{
}

AbstractResource::AbstractResource( const URI& uri )
    : uri_( uri )
{
}

AbstractResource::~AbstractResource()
{
}

std::shared_ptr<EResourceSet> AbstractResource::getResourceSet() const
{
    return resourceSet_.lock();
}

const URI& AbstractResource::getURI() const
{
    return uri_;
}

void AbstractResource::setURI( const URI& uri )
{
    URI oldURI = uri_;
    uri_ = uri;
    if( eNotificationRequired() )
        eNotify( std::make_shared<Notification>( thisPtr_, Notification::SET, RESOURCE__URI, oldURI, uri_ ) );
}

std::shared_ptr<EList<std::shared_ptr<EObject>>> AbstractResource::getContents() const
{
    return eContents_;
}

std::shared_ptr<const ECollectionView<std::shared_ptr<EObject>>> AbstractResource::getAllContents() const
{
    return std::make_shared<ECollectionView<std::shared_ptr<ecore::EObject>>>( getContents() );
}

std::shared_ptr<EObject> AbstractResource::getEObject( const std::string& uriFragment ) const
{
    auto id = uriFragment;
    auto size = uriFragment.size();
    if( !uriFragment.empty() )
    {
        if( uriFragment.at( 0 ) == '/' )
        {
            auto path = split( uriFragment, "/" );
            path.erase( path.begin() );
            return getObjectByPath( path );
        }
        else if( uriFragment.at( size - 1 ) == '?' )
        {
            auto index = uriFragment.find_last_of( '?', size - 2 );
            if( index != std::string::npos )
                id = uriFragment.substr( 0, index );
        }
    }
    return getObjectByID( id );
}

std::string AbstractResource::getURIFragment( const std::shared_ptr<EObject>& eObject ) const
{
    std::string id = EcoreUtils::getID( eObject );
    if( id.empty() )
    {
        auto eCurrent = eObject;
        if(eCurrent->getInternal().eDirectResource() == getThisPtr() )
            return "/" + getURIFragmentRootSegment( eObject );
        else
        {
            std::deque<std::string> fragmentPath;
            auto isContained = false;
            for( auto eContainer = eCurrent->eContainer(); eContainer; eContainer = eCurrent->eContainer())
            {
                if( id.empty() )
                {
                    auto segment = eContainer->getInternal().eURIFragmentSegment(eCurrent->eContainingFeature(), eCurrent);
                    fragmentPath.push_front( segment );
                }

                eCurrent = eContainer;

                if( eContainer->getInternal().eDirectResource() == getThisPtr() )
                {
                    isContained = true;
                    break;
                }
            }
            if( !isContained )
                return "/-1";

            fragmentPath.push_front( id.empty() ? getURIFragmentRootSegment(eCurrent) : "?" + id );
            fragmentPath.push_front( "" );
            return join( fragmentPath, "/" );
        }
    }
    else
        return id;
}

std::string AbstractResource::getURIFragmentRootSegment( const std::shared_ptr<EObject>& eObject ) const
{
    auto contents = eContents_.get();
    return contents->size() > 1 ? std::to_string( contents->indexOf( eObject ) ) : "";
}

std::shared_ptr<EObject> AbstractResource::getObjectByPath( const std::vector<std::string>& uriFragmentPath ) const
{
    auto eObject = getObjectForRootSegment( uriFragmentPath.empty() ? "" : uriFragmentPath[0] );
    for( int i = 1; i < uriFragmentPath.size() && eObject; ++i )
        eObject = eObject->getInternal().eObjectForFragmentSegment( uriFragmentPath[i] );
    return eObject;
}

std::shared_ptr<EObject> AbstractResource::getObjectByID( const std::string& id ) const
{
    auto allContents = getAllContents();
    for( auto eObject : *allContents )
    {
        auto objectID = EcoreUtils::getID( eObject );
        if( id == objectID )
            return eObject;
    }
    return std::shared_ptr<EObject>();
}

std::shared_ptr<EObject> AbstractResource::getObjectForRootSegment( const std::string& rootSegment ) const
{
    int position = 0;
    if( !rootSegment.empty() )
    {
        if( rootSegment.at( 0 ) == '?' )
            return getObjectByID( rootSegment.substr( 1 ) );
        else
            position = std::stoi( rootSegment );
    }
    return position >= 0 && position < getContents()->size() ? getContents()->get( position ) : std::shared_ptr<EObject>();
}

void AbstractResource::attached( const std::shared_ptr<EObject>& object )
{
}

void AbstractResource::detached( const std::shared_ptr<EObject>& object )
{
}

void AbstractResource::load()
{
    if( !isLoaded_ )
    {
        auto uriConverter = getURIConverter();
        auto is = uriConverter->createInputStream( uri_ );
        if( is )
            load( *is );
    }
}

void AbstractResource::load( std::istream& is )
{
    if( !isLoaded_ )
    {
        auto notifications = basicSetLoaded( true, nullptr );

        doLoad( is );

        if( notifications )
            notifications->dispatch();
    }
}

void AbstractResource::unload()
{
    if( isLoaded_ )
    {
        auto notifications = basicSetLoaded( false, nullptr );

        doUnload();

        if( notifications )
            notifications->dispatch();
    }
}

bool AbstractResource::isLoaded() const
{
    return isLoaded_;
}

void AbstractResource::save()
{
    auto uriConverter = getURIConverter();
    auto os = uriConverter->createOutputStream( uri_ );
    if( os )
        save( *os );
}

void AbstractResource::save( std::ostream& os )
{
    doSave( os );
}

std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> AbstractResource::getErrors() const
{
    return errors_;
}

std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> AbstractResource::getWarnings() const
{
    return warnings_;
}

std::shared_ptr<ENotificationChain> AbstractResource::basicSetLoaded( bool isLoaded, const std::shared_ptr<ENotificationChain>& msgs )
{
    auto notifications = msgs;
    bool oldLoaded = isLoaded_;
    isLoaded_ = isLoaded;
    if( eNotificationRequired() )
    {
        if( !notifications )
            notifications = std::make_shared<NotificationChain>();

        notifications->add( std::make_shared<Notification>( thisPtr_, Notification::SET, RESOURCE__IS_LOADED, oldLoaded, isLoaded_ ) );
    }
    return notifications;
}

std::shared_ptr<ENotificationChain> AbstractResource::basicSetResourceSet( const std::shared_ptr<EResourceSet> resourceSet,
                                                                           const std::shared_ptr<ENotificationChain>& msgs )
{
    auto notifications = msgs;
    auto oldAbstractResourceSet = resourceSet_.lock();
    if( oldAbstractResourceSet )
    {
        auto list = std::static_pointer_cast<ENotifyingList<std::shared_ptr<EResource>>>( oldAbstractResourceSet->getResources() );
        notifications = list->add( thisPtr_.lock(), notifications );
    }
    resourceSet_ = resourceSet;
    if( eNotificationRequired() )
    {
        if( !notifications )
            notifications = std::make_shared<NotificationChain>();

        notifications->add(
            std::make_shared<Notification>( thisPtr_, Notification::SET, RESOURCE__RESOURCE_SET, oldAbstractResourceSet, resourceSet ) );
    }
    return notifications;
}

void AbstractResource::doUnload()
{
    eContents_->clear();
}

std::shared_ptr<URIConverter> AbstractResource::getURIConverter() const
{
    auto resourceSet = resourceSet_.lock();
    if( resourceSet )
        return resourceSet->getURIConverter();
    else
    {
        static std::shared_ptr<URIConverter> defaultURIConverter = std::make_shared<ResourceURIConverter>();
        return defaultURIConverter;
    }

    return std::shared_ptr<URIConverter>();
}

std::shared_ptr<EList<std::shared_ptr<EObject>>> AbstractResource::initContents()
{
    class ContentsEList : public AbstractENotifyingList<ENotifyingList<std::shared_ptr<EObject>>, std::shared_ptr<EObject>>
    {
    public:
        ContentsEList( AbstractResource& resource )
            : resource_( resource )
        {
        }

        virtual ~ContentsEList()
        {
        }

        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return resource_.getThisPtr();
        }

        virtual int getFeatureID() const
        {
            return EResource::RESOURCE__CONTENTS;
        }

    protected:
        virtual std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EObject>& eObject,
                                                                const std::shared_ptr<ENotificationChain>& n ) const
        {
            auto notifications = n;
            notifications = eObject->getInternal().eSetResource( resource_.getThisPtr(), notifications );
            resource_.attached( eObject );
            return notifications;
        }

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EObject>& eObject,
                                                                   const std::shared_ptr<ENotificationChain>& n ) const
        {
            auto notifications = n;
            resource_.detached( eObject );
            notifications = eObject->getInternal().eSetResource( nullptr, notifications );
            return notifications;
        }

    private:
        AbstractResource& resource_;
    };

    return std::make_shared<ContentsEList>( *this );
}

std::shared_ptr<EList<std::shared_ptr<EDiagnostic>>> AbstractResource::initDiagnostics()
{
    return std::make_shared<ArrayEList<std::shared_ptr<EDiagnostic>>>();
}
