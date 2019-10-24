#include "ecore/impl/BasicEObject.hpp"
#include "ecore/Constants.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/EClass.hpp"
#include "ecore/ECollectionView.hpp"
#include "ecore/ENotifyingList.hpp"
#include "ecore/EOperation.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EResource.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/impl/ArrayEList.hpp"
#include "ecore/impl/EClassImpl.hpp"
#include "ecore/impl/Notification.hpp"

#include <sstream>
#include <string>

using namespace ecore;
using namespace ecore::impl;

BasicEObject::BasicEObject()
    : eContainer_()
    , eContainerFeatureID_( -1 )
{
}

BasicEObject::~BasicEObject()
{
}

std::shared_ptr<const ECollectionView<std::shared_ptr<ecore::EObject>>> BasicEObject::eAllContents() const
{
    return std::make_shared<ECollectionView<std::shared_ptr<ecore::EObject>>>( eContents() );
}

std::shared_ptr<const EList<std::shared_ptr<ecore::EObject>>> BasicEObject::eContents() const {
    return eContentsList( eClass()->getEContainments() );
}

std::shared_ptr<const EList<std::shared_ptr<ecore::EObject>>> BasicEObject::eCrossReferences() const
{
    return eContentsList( eClass()->getECrossReferences() );
}

std::shared_ptr<const EList<std::shared_ptr<EObject>>> ecore::impl::BasicEObject::eContentsList(const std::shared_ptr<const EList<std::shared_ptr<ecore::EReference>>>& refs ) const
{
    std::vector<std::shared_ptr<EObject>> contents;
    for( auto ref : *refs )
    {
        if( eIsSet( ref ) )
        {
            auto value = eGet( ref );
            if( ref->isMany() )
            {
                std::shared_ptr<EList<std::shared_ptr<EObject>>> l = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>( value );
                std::copy( l->begin(), l->end(), std::back_inserter( contents ) );
            }
            else if( !value.empty() )
            {
                std::shared_ptr<EObject> object = anyCast<std::shared_ptr<EObject>>( value );
                if( object )
                    contents.push_back( object );
            }
        }
    }
    return std::make_shared<ImmutableEList<std::shared_ptr<EObject>>>( std::move( contents ) );
}



std::shared_ptr<ecore::EClass> BasicEObject::eClass() const
{
    return eStaticClass();
}

std::shared_ptr<EClass> BasicEObject::eStaticClass() const
{
    return EcorePackage::eInstance()->getEObject();
}

std::shared_ptr<ecore::EObject> BasicEObject::eContainer() const
{
    return eContainer_.lock();
}
int BasicEObject::eContainerFeatureID() const
{
    return eContainerFeatureID_;
}

std::shared_ptr<EObject> BasicEObject::eObjectForFragmentSegment( const std::string& uriSegment) const
{
    std::size_t index = std::string::npos;
    if (std::isdigit(uriSegment.back()))
    {
        index = uriSegment.find_last_of('.');
        if (index != std::string::npos)
        {
            auto position = std::stoi(uriSegment.substr(index + 1 ));
            auto eFeatureName = uriSegment.substr(0, index);
            auto eFeature = eStructuralFeature(eFeatureName);
            auto value = eGet(eFeature);
            auto list = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>(value);
            if (position < list->size())
                return list->get(position);
        }
    }
    if (index == std::string::npos)
    {
        auto eFeature = eStructuralFeature(uriSegment);
        auto value =  eGet(eFeature);
        return anyCast<std::shared_ptr<EObject>>(value);
    }
    return std::shared_ptr<EObject>();
}

std::shared_ptr<EStructuralFeature> BasicEObject::eStructuralFeature( const std::string& name) const
{
    auto eFeature = eClass()->getEStructuralFeature(name);
    if (!eFeature)
        throw std::runtime_error("The feature " + name + " is not a valid feature");
    return eFeature;
}

std::string BasicEObject::eURIFragmentSegment(const std::shared_ptr<EStructuralFeature>& eFeature, const std::shared_ptr<EObject>& eObject) const
{
    std::stringstream s;
    s << "@";
    s << eFeature->getName();
    if (eFeature->isMany()) {
        auto v = eGet(eFeature, false);
        auto l = anyCast<std::shared_ptr<EList<std::shared_ptr<EObject>>>>(v);
        auto index = l->indexOf(eObject);
        s << ".";
        s << index;
    }
    return s.str();
}

std::shared_ptr<ecore::EStructuralFeature> BasicEObject::eContainingFeature() const
{
    auto eContainer = eContainer_.lock();
    if( eContainer )
    {
        return eContainerFeatureID_ <= EOPPOSITE_FEATURE_BASE
                   ? eContainer->eClass()->getEStructuralFeature( EOPPOSITE_FEATURE_BASE - eContainerFeatureID_ )
                   : std::dynamic_pointer_cast<EReference>( eClass()->getEStructuralFeature( eContainerFeatureID_ ) )->getEOpposite();
    }
    return std::shared_ptr<ecore::EStructuralFeature>();
}

std::shared_ptr<ecore::EReference> BasicEObject::eContainmentFeature() const
{
    return eContainmentFeature( getThisPtr(), eContainer_.lock(), eContainerFeatureID_ );
}

std::shared_ptr<EReference> BasicEObject::eContainmentFeature( const std::shared_ptr<EObject>& eObject,
                                                               const std::shared_ptr<EObject>& eContainer,
                                                               int eContainerFeatureID )
{
    if( eContainer )
    {
        if( eContainerFeatureID <= EOPPOSITE_FEATURE_BASE )
        {
            auto eFeature = eContainer->eClass()->getEStructuralFeature( EOPPOSITE_FEATURE_BASE - eContainerFeatureID );
            if( auto eReference = std::dynamic_pointer_cast<EReference>( eFeature ) )
                return eReference;
        }
        else
        {
            auto eFeature = eObject->eClass()->getEStructuralFeature( eContainerFeatureID );
            if( auto eReference = std::dynamic_pointer_cast<EReference>( eFeature ) )
                return eReference->getEOpposite();
        }
        throw "The containment feature could not be located";
    }
    return std::shared_ptr<EReference>();
}

bool BasicEObject::eIsProxy() const
{
    return static_cast<bool>( eProxyURI_ );
}

std::shared_ptr<EResource> BasicEObject::eResource() const
{
    auto eResource = eResource_.lock();
    if( !eResource )
    {
        auto eContainer = eContainer_.lock();
        if( eContainer )
            eResource = eContainer->eResource();
    }
    return eResource;
}

std::shared_ptr<EResource> BasicEObject::eDirectResource() const
{
    return eResource_.lock();
}

std::shared_ptr<ENotificationChain> BasicEObject::eSetResource( const std::shared_ptr<EResource>& newResource,
                                                                const std::shared_ptr<ENotificationChain>& n )
{
    auto notifications = n;
    auto oldResource = eResource_.lock();
    auto thisPtr = thisPtr_.lock();
    if( oldResource && newResource )
    {
        auto list = std::dynamic_pointer_cast<ENotifyingList<std::shared_ptr<EObject>>>( oldResource->getContents() );
        _ASSERTE( list );
        notifications = list->remove( thisPtr, notifications );

        oldResource->detached( getThisPtr() );
    }

    auto eContainer = eContainer_.lock();
    if( eContainer )
    {
        if( eContainmentFeature()->isResolveProxies() )
        {
            auto oldContainerResource = eContainer->eResource();
            if( oldContainerResource )
            {
                // If we're not setting a new resource, attach it to the old container's resource.
                if( !newResource )
                    oldContainerResource->attached( thisPtr );
                // If we didn't detach it from an old resource already, detach it from the old container's resource.
                else if( !oldResource )
                    oldContainerResource->detached( thisPtr );
            }
        }
        else
        {
            notifications = eBasicRemoveFromContainer( notifications );
            notifications = eBasicSetContainer( nullptr, -1, notifications );
        }
    }

    eResource_ = newResource;

    return notifications;
}

Any BasicEObject::eGet( const std::shared_ptr<EStructuralFeature>& feature ) const
{
    return eGet( feature, true );
}

Any BasicEObject::eGet( const std::shared_ptr<EStructuralFeature>& feature, bool resolve ) const
{
    return eGet( feature, resolve, true );
}

int BasicEObject::eDerivedStructuralFeatureID( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    VERIFYN( eClass()->getEAllStructuralFeatures()->contains( eStructuralFeature ),
             "The feature '%s' is not a valid feature",
             eStructuralFeature->getName().c_str() );
    return eStructuralFeature->getFeatureID();
}

int BasicEObject::eDerivedOperationID( const std::shared_ptr<EOperation>& eOperation ) const
{
    VERIFYN( eClass()->getEAllOperations()->contains( eOperation ),
             "The operation '%s' is not a valid operation",
             eOperation->getName().c_str() );
    return eOperation->getOperationID();
}

Any BasicEObject::eGet( const std::shared_ptr<EStructuralFeature>& eFeature, bool resolve, bool coreType ) const
{
    int featureID = eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 )
        return eGet( featureID, resolve, coreType );
    throw "The feature '" + eFeature->getName() + "' is not a valid feature";
}

Any BasicEObject::eGet( int featureID, bool resolve, bool coreType ) const
{
    std::shared_ptr<EStructuralFeature> eFeature = eClass()->getEStructuralFeature( featureID );
    VERIFYN( eFeature, "Invalid featureID: %i ", featureID );
    return Any();
}

bool BasicEObject::eIsSet( const std::shared_ptr<EStructuralFeature>& eFeature ) const
{
    int featureID = eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 )
        return eIsSet( featureID );
    throw "The feature '" + eFeature->getName() + "' is not a valid feature";
}

bool BasicEObject::eIsSet( int featureID ) const
{
    std::shared_ptr<EStructuralFeature> eFeature = eClass()->getEStructuralFeature( featureID );
    VERIFYN( eFeature, "Invalid featureID: %i ", featureID );
    return false;
}

void BasicEObject::eSet( const std::shared_ptr<EStructuralFeature>& eFeature, const Any& newValue )
{
    int featureID = eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 )
        eSet( featureID, newValue );
    else
        throw "The feature '" + eFeature->getName() + "' is not a valid feature";
}

void BasicEObject::eSet( int featureID, const Any& newValue )
{
    std::shared_ptr<EStructuralFeature> eFeature = eClass()->getEStructuralFeature( featureID );
    VERIFYN( eFeature, "Invalid featureID: %i ", featureID );
}

void BasicEObject::eUnset( const std::shared_ptr<EStructuralFeature>& eFeature )
{
    int featureID = eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 )
        eUnset( featureID );
    else
        throw "The feature '" + eFeature->getName() + "' is not a valid feature";
}

void BasicEObject::eUnset( int featureID )
{
    std::shared_ptr<EStructuralFeature> eFeature = eClass()->getEStructuralFeature( featureID );
    VERIFYN( eFeature, "Invalid featureID: %i ", featureID );
}

Any BasicEObject::eInvoke( const std::shared_ptr<EOperation>& eOperation, const std::shared_ptr<EList<Any>>& arguments )
{
    int operationID = eDerivedOperationID( eOperation );
    if( operationID >= 0 )
        return eInvoke( operationID, arguments );
    throw "The operation '" + eOperation->getName() + "' is not a valid operation";
}

Any BasicEObject::eInvoke( int operationID, const std::shared_ptr<EList<Any>>& arguments )
{
    std::shared_ptr<EOperation> eOperation = eClass()->getEOperation( operationID );
    VERIFYN( eOperation, "Invalid operationID: %i ", operationID );
    return Any();
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd,
                                                                    int featureID,
                                                                    const std::shared_ptr<ENotificationChain>& notifications )
{
    return notifications;
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd,
                                                                       int featureID,
                                                                       const std::shared_ptr<ENotificationChain>& notifications )
{
    return notifications;
}

std::shared_ptr<ENotificationChain> BasicEObject::eInverseAdd( const std::shared_ptr<EObject>& otherEnd,
                                                               int featureID,
                                                               const std::shared_ptr<ENotificationChain>& n )
{
    auto notifications = n;
    if( featureID >= 0 )
        return eBasicInverseAdd( otherEnd, featureID, notifications );
    else
    {
        notifications = eBasicRemoveFromContainer( notifications );
        return eBasicSetContainer( otherEnd, featureID, notifications );
    }
}

std::shared_ptr<ENotificationChain> BasicEObject::eInverseRemove( const std::shared_ptr<EObject>& otherEnd,
                                                                  int featureID,
                                                                  const std::shared_ptr<ENotificationChain>& notifications )
{
    if( featureID >= 0 )
        return eBasicInverseRemove( otherEnd, featureID, notifications );
    else
        return eBasicSetContainer( nullptr, featureID, notifications );
}

URI BasicEObject::eProxyURI() const
{
    return eProxyURI_.value_or( URI() );
}

void BasicEObject::eSetProxyURI( const URI& uri )
{
    eProxyURI_ = uri;
}

std::shared_ptr<EObject> BasicEObject::eResolveProxy( const std::shared_ptr<EObject>& proxy ) const
{
    return std::shared_ptr<EObject>();
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicSetContainer( const std::shared_ptr<EObject>& newContainer,
                                                                      int newContainerFeatureID,
                                                                      const std::shared_ptr<ENotificationChain>& n )
{
    auto notifications = n;
    auto thisPtr = thisPtr_.lock();
    auto oldContainer = eContainer_.lock();
    auto oldResource = eResource_.lock();

    // resource
    std::shared_ptr<EResource> newResource;
    if( oldResource )
    {
        if( newContainer && !eContainmentFeature( thisPtr, newContainer, newContainerFeatureID ) )
        {
            auto list = std::dynamic_pointer_cast<ENotifyingList<std::shared_ptr<EObject>>>( oldResource->getContents() );
            _ASSERTE( list );
            notifications = list->remove( thisPtr, notifications );

            eResource_.reset();

            newResource = newContainer->eResource();
        }
        else
            oldResource = nullptr;
    }
    else
    {
        if( oldContainer )
            oldResource = oldContainer->eResource();

        if( newContainer )
            newResource = newContainer->eResource();
    }

    if( oldResource && oldResource != newResource )
        oldResource->detached( thisPtr );

    if( newResource && newResource != oldResource )
        newResource->attached( thisPtr );

    // basic set
    int oldContainerFeatureID = eContainerFeatureID_;
    eContainer_ = newContainer;
    eContainerFeatureID_ = newContainerFeatureID;
    eResource_ = newResource;

    // notification
    if( eNotificationRequired() )
    {
        if( oldContainer && oldContainerFeatureID >= 0 && oldContainerFeatureID != newContainerFeatureID )
        {
            auto notification = std::make_shared<Notification>(
                getThisPtr(), ENotification::SET, oldContainerFeatureID, oldContainer, std::shared_ptr<EObject>() );
            if( notifications )
                notifications->add( notification );
            else
                notifications = notification;
        }
        if( newContainerFeatureID >= 0 )
        {
            auto notification = std::make_shared<Notification>( getThisPtr(),
                                                                ENotification::SET,
                                                                newContainerFeatureID,
                                                                oldContainerFeatureID == newContainerFeatureID ? oldContainer
                                                                                                               : std::shared_ptr<EObject>(),
                                                                newContainer );
            if( notifications )
                notifications->add( notification );
            else
                notifications = notification;
        }
    }
    return notifications;
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicRemoveFromContainer( const std::shared_ptr<ENotificationChain>& notifications )
{
    if( eContainerFeatureID_ >= 0 )
        return eBasicRemoveFromContainerFeature( notifications );
    else
    {
        auto eContainer = eContainer_.lock();
        if( eContainer )
            return eContainer->eInverseRemove( getThisPtr(), EOPPOSITE_FEATURE_BASE - eContainerFeatureID_, notifications );
    }
    return notifications;
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicRemoveFromContainerFeature(
    const std::shared_ptr<ENotificationChain>& notifications )
{
    auto reference = std::dynamic_pointer_cast<EReference>( eClass()->getEStructuralFeature( eContainerFeatureID_ ) );
    if( reference )
    {
        auto inverseFeature = reference->getEOpposite();
        auto container = eContainer_.lock();
        if( container && inverseFeature )
            return container->eInverseRemove( getThisPtr(), inverseFeature->getFeatureID(), notifications );
    }
    return notifications;
}
