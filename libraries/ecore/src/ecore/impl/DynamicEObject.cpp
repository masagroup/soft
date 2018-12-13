#include "ecore/impl/DynamicEObject.hpp"
#include "ecore/impl/WeakPtr.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EList.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EReference.hpp"
#include "ecore/impl/ArrayEList.hpp"
#include "ecore/impl/EObjectEList.hpp"
#include "ecore/impl/Proxy.hpp"

#include <string>
#include <sstream>

using namespace ecore;
using namespace ecore::impl;

using EObjectProxy = Proxy< std::shared_ptr<EObject> >;

class DynamicEObject::FeaturesAdapter : public EAdapter
{
public:
    FeaturesAdapter( DynamicEObject& eObject )
        : eObject_( eObject )
    {
    }

    virtual ~FeaturesAdapter()
    {

    }

    virtual void notifyChanged( const std::shared_ptr<ENotification>& notification )
    {
        int eventType = notification->getEventType();
        auto eNotifier = std::dynamic_pointer_cast<DynamicEObject>( notification->getNotifier() );
        if( eventType != ENotification::REMOVING_ADAPTER )
        {
            int featureID = notification->getFeatureID();
            if( featureID == EcorePackage::ECLASS__ESTRUCTURAL_FEATURES )
                eObject_.resizeProperties();
        }
    }

private:
    DynamicEObject& eObject_;
};

DynamicEObject::DynamicEObject()
    : featuresAdapter_( new FeaturesAdapter( *this ) )
{
}

DynamicEObject::DynamicEObject( const std::shared_ptr<EClass>& eClass )
    : featuresAdapter_( new FeaturesAdapter( *this ) )
{
    setEClass( eClass );
}

DynamicEObject::~DynamicEObject()
{
    setEClass( nullptr );
}

std::shared_ptr<ecore::EClass> DynamicEObject::eClass() const
{
    return is_uninitialized( eClass_ ) ? eStaticClass() : eClass_.lock();
}

void DynamicEObject::setEClass( const std::shared_ptr<EClass>& newClass )
{
    if( auto eClass = eClass_.lock() )
        eClass->eAdapters().remove( featuresAdapter_.get() );

    eClass_ = newClass;
    resizeProperties();

    if ( auto eClass = eClass_.lock() )
        eClass->eAdapters().add( featuresAdapter_.get() );
}

Any DynamicEObject::eGet( int featureID, bool resolve, bool coreType ) const
{
    int dynamicFeatureID = featureID - eStaticFeatureCount();
    if( dynamicFeatureID >= 0 )
    {
        auto eFeature = eDynamicFeature( featureID );

        // retrieve value or compute it if empty
        auto result = properties_[ dynamicFeatureID ];
        if( result.empty() )
        {
            if( eFeature->isMany() )
                properties_[ dynamicFeatureID ] = result = createList( eFeature );
            else if ( eFeature->eIsProxy() )
                properties_[ dynamicFeatureID ] = result = std::make_shared<EObjectProxy>();
        }
        
        // proxy 
        if( eFeature->eIsProxy() )
        {
            auto proxy = anyCast<std::shared_ptr<EObjectProxy>>( result );
            result = proxy->get();
        }
        
        return result;
    }
    return BasicEObject::eGet( featureID, resolve , coreType );
}

bool DynamicEObject::eIsSet( int featureID ) const
{
    int dynamicFeatureID = featureID - eStaticFeatureCount();
    if( dynamicFeatureID >= 0 )
        return !properties_[ dynamicFeatureID ].empty();
    else
        return BasicEObject::eIsSet( featureID );
}

void DynamicEObject::eSet( int featureID, const Any & newValue )
{
    int dynamicFeatureID = featureID - eStaticFeatureCount();
    if( dynamicFeatureID >= 0 )
    {
        auto dynamicFeature = eDynamicFeature( featureID );
        if( isContainer( dynamicFeature ) )
        {
            _ASSERT_EXPR( newValue.type() == typeid(std::shared_ptr<EObject>()), " Feature defined as a container : value must be a std::shared_ptr<EObject>" );

            // container
            std::shared_ptr<EObject> newContainer = anyCast<std::shared_ptr<EObject>>( newValue );
            if( newContainer != eContainer() || ( newContainer && eContainerFeatureID() != featureID ) )
            {
                std::shared_ptr<ENotificationChain> notifications;
                if( eContainer() )
                    notifications = eBasicRemoveFromContainer( notifications );
                if( newContainer )
                    notifications = newContainer->eInverseAdd( getThisPtr(), featureID, notifications );
                notifications = eBasicSetContainer( newContainer, featureID, notifications );
                if( notifications )
                    notifications->dispatch();
            }
            else if( eNotificationRequired() )
                eNotify( std::make_shared<Notification>( getThisPtr(), Notification::SET, featureID, newValue, newValue ) );
        }
        else if( isBidirectional( dynamicFeature ) || isContains( dynamicFeature ) )
        {
            _ASSERT_EXPR( newValue.type() == typeid( std::shared_ptr<EObject>() ), " Feature defined as birectional or containment : value must be a std::shared_ptr<EObject>" );

            // inverse - opposite
            auto oldValue = properties_[ dynamicFeatureID ];
            if( oldValue != newValue )
            {
                std::shared_ptr<ENotificationChain> notifications;
                auto oldObject = dynamicFeature->eIsProxy() ? anyCast<std::shared_ptr<EObjectProxy>>( oldValue )->get()  
                                                            : anyCast<std::shared_ptr<EObject>>( oldValue );
                auto newObject = anyCast<std::shared_ptr<EObject>>( newValue );
                
                if( !isBidirectional( dynamicFeature ) )
                {
                    if( oldObject )
                        notifications = oldObject->eInverseRemove( getThisPtr(), EOPPOSITE_FEATURE_BASE - featureID, notifications );
                        
                    if ( newObject )
                        notifications = newObject->eInverseAdd( getThisPtr(), EOPPOSITE_FEATURE_BASE - featureID, notifications );
                }
                else
                {
                    auto dynamicReference = std::dynamic_pointer_cast<EReference>( dynamicFeature );
                    auto reverseFeature = dynamicReference->getEOpposite();
                    if( oldObject )
                        notifications = oldObject->eInverseRemove( getThisPtr(), reverseFeature->getFeatureID(), notifications );

                    if( newObject )
                        notifications = newObject->eInverseAdd( getThisPtr(), reverseFeature->getFeatureID(), notifications );
                }
                // basic set
                if( dynamicFeature->eIsProxy() )
                {
                    auto proxy = anyCast < std::shared_ptr<EObjectProxy> >( oldValue );
                    proxy->set( newObject );
                }
                else
                    properties_[ dynamicFeatureID ] = newValue;
                
                // create notification
                if( eNotificationRequired() )
                {
                    auto notification = std::make_shared<Notification>( getThisPtr(), Notification::SET, featureID, oldValue, newValue );
                    if( notifications )
                        notifications->add( notification );
                    else
                        notifications = notification;
                }

                // notify
                if( notifications )
                    notifications->dispatch();
            }

        }
        else
        {
            // basic set
            auto oldValue = properties_[ dynamicFeatureID ];

            if( dynamicFeature->eIsProxy() )
            {
                _ASSERT_EXPR( newValue.type() == typeid( std::shared_ptr<EObject>() ), " Feature defined as reference proxy : value must be a std::shared_ptr<EObject>" );

                auto newObject = anyCast<std::shared_ptr<EObject>>( newValue );
                auto proxy = anyCast < std::shared_ptr<EObjectProxy> >( oldValue );
                proxy->set( newObject );
            }
            else
                properties_[ dynamicFeatureID ] = newValue;

            // notify
            if( eNotificationRequired() )
                eNotify( std::make_shared<Notification>( getThisPtr(), Notification::SET, featureID, oldValue, newValue ));
        }
    }
    else
        BasicEObject::eSet( featureID, newValue );
}

void DynamicEObject::eUnset( int featureID )
{
    int dynamicFeatureID = featureID - eStaticFeatureCount();
    if( dynamicFeatureID >= 0 )
        properties_[ dynamicFeatureID ].reset();
    else
        BasicEObject::eUnset( featureID );
}

Any DynamicEObject::eInvoke( int operationID, const std::shared_ptr<EList<Any>>& arguments )
{
    return Any();
}

int DynamicEObject::eStaticFeatureCount() const
{
    return eStaticClass()->getFeatureCount();
}

int DynamicEObject::eStaticOperationCount() const
{
    return eStaticClass()->getOperationCount();
}

int DynamicEObject::eDynamicFeatureID( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    return eClass()->getFeatureID( eStructuralFeature ) - eStaticFeatureCount();
}

std::shared_ptr<EStructuralFeature> DynamicEObject::eDynamicFeature( int dynamicFeatureID ) const
{
    return eClass()->getEStructuralFeature( dynamicFeatureID + eStaticFeatureCount() );
}

bool DynamicEObject::isBidirectional( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    if( auto eReference = std::dynamic_pointer_cast<EReference>( eStructuralFeature ) )
        return static_cast<bool>(eReference->getEOpposite());
    return false;
}

bool DynamicEObject::isContainer( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    if( auto eReference = std::dynamic_pointer_cast<EReference>( eStructuralFeature ) )
    {
        if( auto opposite = eReference->getEOpposite() )
            return static_cast<bool>( opposite->isContainment() );
    }
    return false;
}

bool DynamicEObject::isContains( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    if( auto eReference = std::dynamic_pointer_cast<EReference>( eStructuralFeature ) )
        return eReference->isContainment();
    return false;
}

void DynamicEObject::resizeProperties()
{
    properties_.resize( eClass()->getFeatureCount() - eStaticFeatureCount() );
}

std::shared_ptr<EList<std::shared_ptr<EObject>>> DynamicEObject::createList( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    if( auto eAttribute = std::dynamic_pointer_cast<EAttribute>( eStructuralFeature ) )
    {
        if( eAttribute->isUnique() )
            return std::make_shared< ArrayEList<std::shared_ptr<EObject>, true> >();
        else
            return std::make_shared< ArrayEList<std::shared_ptr<EObject>, false> >();
    }
    else if( auto eReference = std::dynamic_pointer_cast<EReference>( eStructuralFeature ) )
    {
        if( eReference->isContainment() )
        {
            // containment

            if( auto eReverseFeature = eReference->getEOpposite() )
            {
                //  opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, true, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, true, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );

                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, false, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, false, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                }
            }
            else
            {
                // no opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, true, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, true, false>>( getThisPtr(), eReference->getFeatureID() );
                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, false, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, false, false>>( getThisPtr(), eReference->getFeatureID() );
                }
            }
        }
        else
        {
            // no containment

            if( auto eReverseFeature = eReference->getEOpposite() )
            {
                //  opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, true, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, true, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );

                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, false, true>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, false, false>>( getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID() );
                }
            }
            else
            {
                // no opposite

                if( eReference->eIsProxy() )
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, true, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, true, false>>( getThisPtr(), eReference->getFeatureID() );
                }
                else
                {
                    if( eReference->isUnsettable() )
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, false, true>>( getThisPtr(), eReference->getFeatureID() );
                    else
                        return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, false, false>>( getThisPtr(), eReference->getFeatureID() );
                }
            }
        }
    }
    return std::shared_ptr< EList<std::shared_ptr<EObject>> >();
}
