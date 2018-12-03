#include "ecore/impl/DynamicEObject.hpp"
#include "ecore/impl/WeakPtr.hpp"
#include "ecore/EClass.hpp"

#include <boost/assert.hpp>
#include <string>
#include <sstream>

using namespace ecore;
using namespace ecore::impl;

DynamicEObject::DynamicEObject()
{
}

DynamicEObject::DynamicEObject( const std::shared_ptr<EClass>& eClass )
    : eClass_(eClass)
{
}

DynamicEObject::~DynamicEObject()
{
}



std::shared_ptr<ecore::EClass> ecore::impl::DynamicEObject::eClass() const
{
    return is_uninitialized( eClass_ ) ? eStaticClass() : eClass_.lock();
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

int ecore::impl::DynamicEObject::eStaticFeatureCount() const
{
    return eStaticClass()->getFeatureCount();
}

int ecore::impl::DynamicEObject::eStaticOperationCount() const
{
    return eStaticClass()->getOperationCount();
}
