#include "ecore/impl/BasicEObject.hpp"
#include "ecore/impl/BasicEList.hpp"
#include "ecore/impl/EClassImpl.hpp"
#include "ecore/impl/Notification.hpp"
#include "ecore/Constants.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EOperation.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EcorePackage.hpp"

#include <boost/assert.hpp>
#include <string>
#include <sstream>

using namespace ecore;
using namespace ecore::impl;

BasicEObject::BasicEObject()
    : eDeliver_( false )
    , eAdapters_( new BasicEList<EAdapter*>() )
    , eContainer_()
    , eContainerFeatureID_( -1 )
    , thisPtr_()
{

}

BasicEObject::~BasicEObject()
{

}

void BasicEObject::setThisPtr( const std::shared_ptr<BasicEObject>& thisPtr )
{
    thisPtr_ = thisPtr;
}

std::shared_ptr<BasicEObject> BasicEObject::getThisPtr() const
{
    return thisPtr_.lock();
}


EList<EAdapter*>& BasicEObject::eAdapters()
{
    return *eAdapters_;
}

bool BasicEObject::eDeliver()
{
    return eDeliver_;
}

void BasicEObject::eSetDeliver( bool deliver )
{
    eDeliver_ = deliver;
}

void BasicEObject::eNotify( const std::shared_ptr<ENotification>& notification )
{
    for( auto eAdapter : *eAdapters_ )
        eAdapter->notifyChanged( notification );
}

bool BasicEObject::eNotificationRequired()
{
    return eDeliver_ && eAdapters_->size() > 0;
}

std::shared_ptr<EList<std::shared_ptr<ecore::EObject>>> BasicEObject::eAllContents() const
{
    return std::shared_ptr<EList<std::shared_ptr<ecore::EObject>>>();
}

std::shared_ptr<EList<std::shared_ptr<ecore::EObject>>> BasicEObject::eContents() const
{
    return std::dynamic_pointer_cast<EClassImpl>( eClass()  )->getEAllContainments()->asEListOf<std::shared_ptr<EObject>>();
}

std::shared_ptr<EList<std::shared_ptr<ecore::EObject>>> BasicEObject::eCrossReferences() const
{
    return std::dynamic_pointer_cast<EClassImpl>( eClass() )->getEAllCrossReferences()->asEListOf<std::shared_ptr<EObject>>();
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

std::shared_ptr<ecore::EStructuralFeature> BasicEObject::eContainingFeature() const
{
    auto eContainer = eContainer_.lock();
    if( eContainer )
    {
        return
            eContainerFeatureID_ <= EOPPOSITE_FEATURE_BASE ?
            eContainer->eClass()->getEStructuralFeature( EOPPOSITE_FEATURE_BASE - eContainerFeatureID_ ) :
            std::dynamic_pointer_cast<EReference>( eClass()->getEStructuralFeature( eContainerFeatureID_ ) )->getEOpposite();
    }
    return std::shared_ptr<ecore::EStructuralFeature>();
}

std::shared_ptr<ecore::EReference> BasicEObject::eContainmentFeature() const
{
    return eContainmentFeature( getThisPtr(), eContainer_.lock(), eContainerFeatureID_ );
}


std::shared_ptr<EReference> BasicEObject::eContainmentFeature( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EObject>& eContainer, int eContainerFeatureID )
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
            if ( auto eReference = std::dynamic_pointer_cast<EReference>( eFeature ) )
                return eReference->getEOpposite();
        }
        throw "The containment feature could not be located";
    }
    return std::shared_ptr<EReference>();
}

bool BasicEObject::eIsProxy() const
{
    return false;
}

int BasicEObject::eResource() const
{
    return 0;
}


boost::any BasicEObject::eGet( const std::shared_ptr<EStructuralFeature>& feature ) const
{
    return eGet( feature, true );
}

boost::any BasicEObject::eGet( const std::shared_ptr<EStructuralFeature>& feature, bool resolve ) const
{
    return eGet( feature, true );
}


int BasicEObject::eDerivedStructuralFeatureID( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const
{
    BOOST_VERIFY_MSG( eClass()->getEAllStructuralFeatures()->contains( eStructuralFeature ), ( static_cast<std::ostringstream&>( std::stringstream() << "The feature '" << eStructuralFeature->getName() << "' is not a valid feature" ) ).str().c_str() );
    return eStructuralFeature->getFeatureID();
}

int BasicEObject::eDerivedOperationID( const std::shared_ptr<EOperation>& eOperation ) const
{
    BOOST_VERIFY_MSG( eClass()->getEAllOperations()->contains( eOperation ), ( static_cast<std::ostringstream&>( std::stringstream() << "The operation '" << eOperation->getName() << "' is not a valid operation" ) ).str().c_str() );
    return eOperation->getOperationID();
}

boost::any BasicEObject::eGet( const std::shared_ptr<EStructuralFeature>& eFeature, bool resolve, bool coreType ) const
{
    int featureID = eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 )
        return eGet( featureID, resolve, coreType );
    throw "The feature '" + eFeature->getName() + "' is not a valid feature";
}

boost::any BasicEObject::eGet( int featureID, bool resolve, bool coreType ) const
{
    std::shared_ptr<EStructuralFeature> eFeature = eClass()->getEStructuralFeature( featureID );
    BOOST_ASSERT_MSG( eFeature , ( static_cast<std::ostringstream&>( std::stringstream() << "Invalid featureID:  " << featureID ) ).str().c_str() );
    return boost::any();
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
    BOOST_ASSERT_MSG( eFeature , ( static_cast<std::ostringstream&>( std::stringstream() << "Invalid featureID:  " << featureID ) ).str().c_str() );
    return false;
}

void BasicEObject::eSet( const std::shared_ptr<EStructuralFeature>& eFeature, const boost::any & newValue )
{
    int featureID = eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 )
        eSet( featureID, newValue );
    throw "The feature '" + eFeature->getName() + "' is not a valid feature";
}

void BasicEObject::eSet( int featureID, const boost::any & newValue )
{
    std::shared_ptr<EStructuralFeature> eFeature = eClass()->getEStructuralFeature( featureID );
    BOOST_ASSERT_MSG( eFeature , ( static_cast<std::ostringstream&>( std::stringstream() << "Invalid featureID:  " << featureID ) ).str().c_str() );
}


void BasicEObject::eUnset( const std::shared_ptr<EStructuralFeature>& eFeature )
{
    int featureID = eDerivedStructuralFeatureID( eFeature );
    if( featureID >= 0 )
        eUnset( featureID );
    throw "The feature '" + eFeature->getName() + "' is not a valid feature";
}

void BasicEObject::eUnset( int featureID )
{
    std::shared_ptr<EStructuralFeature> eFeature = eClass()->getEStructuralFeature( featureID );
    BOOST_ASSERT_MSG( eFeature , ( static_cast<std::ostringstream&>( std::stringstream() << "Invalid featureID:  " << featureID ) ).str().c_str() );
}

boost::any BasicEObject::eInvoke( const std::shared_ptr<EOperation>& eOperation, const std::shared_ptr<EList<boost::any>>& arguments )
{
    int operationID = eDerivedOperationID( eOperation );
    if( operationID >= 0 )
        return eInvoke( operationID, arguments );
    throw "The operation '" + eOperation->getName() + "' is not a valid operation";
}

boost::any BasicEObject::eInvoke( int operationID, const std::shared_ptr<EList<boost::any>>& arguments )
{
    std::shared_ptr<EOperation> eOperation = eClass()->getEOperation( operationID );
    BOOST_ASSERT_MSG( eOperation, ( static_cast<std::ostringstream&>( std::stringstream() << "Invalid operationID:  " << operationID ) ).str().c_str() );
    return boost::any();
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    return notifications;
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    return notifications;
}

std::shared_ptr<ENotificationChain> BasicEObject::eInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& n )
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

std::shared_ptr<ENotificationChain> BasicEObject::eInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    if( featureID >= 0 )
        return eBasicInverseRemove( otherEnd, featureID, notifications );
    else
        return eBasicSetContainer( nullptr, featureID, notifications );
}

std::shared_ptr<ENotificationChain> BasicEObject::eBasicSetContainer( const std::shared_ptr<EObject>& newContainer, int newContainerFeatureID, const std::shared_ptr<ENotificationChain>& n )
{
    auto notifications = n;
    auto oldContainer = eContainer_.lock();
    int oldContainerFeatureID = eContainerFeatureID_;
    eContainer_ = newContainer;
    eContainerFeatureID_ = newContainerFeatureID;

    if( eNotificationRequired() )
    {
        if( oldContainer  && oldContainerFeatureID >= 0 && oldContainerFeatureID != newContainerFeatureID )
        {
            auto notification = std::make_shared<Notification>( ENotification::SET,
                                                                getThisPtr(),
                                                                oldContainer->eClass()->getEStructuralFeature( oldContainerFeatureID ),
                                                                oldContainer,
                                                                std::shared_ptr<EObject>() );
            if( notifications )
                notifications->add( notification );
            else
                notifications = notification;
        }
        if( newContainerFeatureID >= 0 )
        {
            auto notification = std::make_shared<Notification>( ENotification::SET,
                                                                getThisPtr(),
                                                                newContainer->eClass()->getEStructuralFeature( newContainerFeatureID ),
                                                                oldContainerFeatureID == newContainerFeatureID ? oldContainer : std::shared_ptr<EObject>(),
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

std::shared_ptr<ENotificationChain> BasicEObject::eBasicRemoveFromContainerFeature( const std::shared_ptr<ENotificationChain>& notifications )
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










