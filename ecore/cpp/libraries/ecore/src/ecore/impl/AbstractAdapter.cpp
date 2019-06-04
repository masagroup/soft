#include "ecore/impl/AbstractAdapter.hpp"
#include "ecore/EList.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/ENotifier.hpp"

using namespace ::ecore;
using namespace ::ecore::impl;

std::shared_ptr<ENotifier> AbstractAdapter::getTarget() const
{
    return target_.lock();
}

void AbstractAdapter::setTarget( const std::shared_ptr<ENotifier>& notifier )
{
    target_ = notifier;
}
