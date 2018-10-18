#include "BasicEObject.hpp"
#include "ecore/BasicEList.hpp"
#include "ecore/EAdapter.hpp"

using namespace ecore;

BasicEObject::BasicEObject()
    : eDeliver_(false)
    , eAdapters_( new BasicEList<EAdapter*>() )
{

}

BasicEObject::~BasicEObject()
{

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

void BasicEObject::eNotify( const ENotification& notification )
{
    for( auto eAdapter : *eAdapters_ )
        eAdapter->notifyChanged( notification );
}

bool BasicEObject::eNotificationRequired()
{
    return eDeliver_ && eAdapters_->size() > 0;
}
