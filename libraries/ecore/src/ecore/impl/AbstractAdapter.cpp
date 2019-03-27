#include "ecore/impl/AbstractAdapter.hpp"
#include "ecore/EList.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/ENotifier.hpp"

using namespace ::ecore;
using namespace ::ecore::impl;

AbstractAdapter::~AbstractAdapter()
{
    if( auto p = target_.lock() )
    {
        auto& adapters = p->eAdapters();
        adapters.remove( this );
        if( adapters.size() == 0 )
            p->eSetDeliver( false );
    }
}

std::shared_ptr<ENotifier> AbstractAdapter::getTarget() const
{
    return target_.lock();
}

void AbstractAdapter::setTarget( const std::shared_ptr<ENotifier>& notifier )
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