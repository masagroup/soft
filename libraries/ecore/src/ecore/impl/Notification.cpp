// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************


#include "ecore/impl/Notification.hpp"
#include "ecore/impl/NotificationChain.hpp"
#include "ecore/Stream.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EStructuralFeature.hpp"

using namespace ecore;
using namespace ecore::impl;

Notification::Notification( const std::shared_ptr<EObject>& notifier
                            , EventType type
                            , const std::shared_ptr<EStructuralFeature>& feature
                            , const Any & oldValue
                            , const Any & newValue
                            , std::size_t position )
    : AbstractNotification( type, oldValue, newValue, position )
    , notifier_( notifier )
    , feature_( feature )
    , featureID_( -1 )
{
}



Notification::Notification( const std::shared_ptr<EObject>& notifier,
              EventType type,
              int featureID,
              const Any& oldValue,
              const Any& newValue,
              std::size_t position )
    : AbstractNotification( type, oldValue, newValue, position )
    , notifier_( notifier )
    , feature_()
    , featureID_( featureID )
{
}

Notification::~Notification()
{
}

std::shared_ptr<ENotifier> Notification::getNotifier() const
{
    return notifier_;
}

std::shared_ptr<EStructuralFeature> Notification::getFeature() const
{
    return feature_ ? feature_ : notifier_->eClass()->getEStructuralFeature( featureID_ );
}

int Notification::getFeatureID() const
{
    return featureID_ != -1 ? featureID_ : ( feature_ ? feature_->getFeatureID() : -1 );
}
