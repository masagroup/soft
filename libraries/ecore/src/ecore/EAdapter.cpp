#include "EAdapter.hpp"
#include "ecore/EList.hpp"
#include "ecore/ENotifier.hpp"
#include "ecore/ENotification.hpp"


using namespace ::ecore;

EAdapter::~EAdapter()
{
    if( auto p = target_.lock() )
    {
        auto& adapters = p->eAdapters();
        adapters.remove( this );
        if( adapters.size() == 0 )
            p->eSetDeliver( false );
    }
}

std::shared_ptr<ENotifier> EAdapter::getTarget()
{
    return target_.lock();
}

void EAdapter::setTarget( const std::shared_ptr<ENotifier>& notifier )
{
    if( auto p = target_.lock() )
    {
        if( p == notifier )
            return;

        auto& adapters = p->eAdapters();
        adapters.remove( this );
        if( adapters.size() == 0 )
            p->eSetDeliver( false );
    }

    target_ = notifier;

    if( auto p = target_.lock() )
    {
        p->eAdapters().add( this );
        p->eSetDeliver( true );
    }
}
