#include "ecore/impl/DynamicEObject.hpp"
#include "ecore/impl/WeakPtr.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EList.hpp"

#include <boost/assert.hpp>
#include <string>
#include <sstream>

using namespace ecore;
using namespace ecore::impl;


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

boost::any DynamicEObject::eGet( int featureID, bool resolve, bool coreType ) const
{
    int dynamicFeatureID = featureID - eStaticFeatureCount();
    if( dynamicFeatureID >= 0 )
        return properties_[ dynamicFeatureID ];
    return BasicEObject::eGet( featureID, resolve , coreType );
}

bool DynamicEObject::eIsSet( int featureID ) const
{
    return false;
}

void DynamicEObject::eSet( int featureID, const boost::any & newValue )
{
}

void DynamicEObject::eUnset( int featureID )
{
}

boost::any DynamicEObject::eInvoke( int operationID, const std::shared_ptr<EList<boost::any>>& arguments )
{
    return boost::any();
}

int DynamicEObject::eStaticFeatureCount() const
{
    return eStaticClass()->getFeatureCount();
}

int DynamicEObject::eStaticOperationCount() const
{
    return eStaticClass()->getOperationCount();
}

void DynamicEObject::resizeProperties()
{
    properties_.resize( eClass()->getFeatureCount() - eStaticFeatureCount() );
}
