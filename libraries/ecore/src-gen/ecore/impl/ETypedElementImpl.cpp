// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

// *****************************************************************************
//    
// Warning: This file was generated by sof.generator.cpp Generator
// 
//*****************************************************************************
#include "ecore/impl/ETypedElementImpl.hpp"
#include "ecore/EAnnotation.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/EGenericType.hpp"
#include "ecore/ENamedElement.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/impl/EObjectEList.hpp"


#include <cassert>
#include <iostream>
#include <sstream>

//Start of user code ETypedElementImpl [declaration-includes]
//End of user code


using namespace ecore;
using namespace ecore::impl;

//Start of user code ETypedElementImpl [declaration-begin]
//End of user code

//*********************************
// Constructor / Destructor
//*********************************
ETypedElementImpl::ETypedElementImpl()
:   ordered_(true)
,   unique_(true)
,   lowerBound_(0)
,   upperBound_(1)
,   eType_([&](){ return initEType();})
,   eGenericType_()
//Start of user code ETypedElementImpl::ETypedElementImpl
{
}
//End of user code


ETypedElementImpl::~ETypedElementImpl()
{
//Start of user code ETypedElementImpl::~ETypedElementImpl
//End of user code
}


std::shared_ptr<EClass> ETypedElementImpl::eStaticClass() const
{
    return EcorePackage::eInstance()->getETypedElement();
}

std::shared_ptr<ETypedElementImpl> ETypedElementImpl::getThisPtr() const
{
    return thisPtr_.lock();
}

void ETypedElementImpl::setThisPtr(const std::shared_ptr<ETypedElementImpl>& thisPtr)
{
    thisPtr_ = thisPtr;
    ENamedElementImpl::setThisPtr(thisPtr);
}

//*********************************
// Attributes
//*********************************
bool ETypedElementImpl::isOrdered() const 
{
    // Start of user code ETypedElementImpl::isOrdered
    return ordered_;
    // End of user code
}


void ETypedElementImpl::setOrdered(bool newOrdered)
{
    // Start of user code ETypedElementImpl::setOrdered
    auto oldOrdered = ordered_;
    ordered_ = newOrdered;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ETYPED_ELEMENT__ORDERED, oldOrdered, newOrdered));
    // End of user code
}


bool ETypedElementImpl::isUnique() const 
{
    // Start of user code ETypedElementImpl::isUnique
    return unique_;
    // End of user code
}


void ETypedElementImpl::setUnique(bool newUnique)
{
    // Start of user code ETypedElementImpl::setUnique
    auto oldUnique = unique_;
    unique_ = newUnique;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ETYPED_ELEMENT__UNIQUE, oldUnique, newUnique));
    // End of user code
}


int ETypedElementImpl::getLowerBound() const 
{
    // Start of user code ETypedElementImpl::getLowerBound
    return lowerBound_;
    // End of user code
}


void ETypedElementImpl::setLowerBound(int newLowerBound)
{
    // Start of user code ETypedElementImpl::setLowerBound
    auto oldLowerBound = lowerBound_;
    lowerBound_ = newLowerBound;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ETYPED_ELEMENT__LOWER_BOUND, oldLowerBound, newLowerBound));
    // End of user code
}


int ETypedElementImpl::getUpperBound() const 
{
    // Start of user code ETypedElementImpl::getUpperBound
    return upperBound_;
    // End of user code
}


void ETypedElementImpl::setUpperBound(int newUpperBound)
{
    // Start of user code ETypedElementImpl::setUpperBound
    auto oldUpperBound = upperBound_;
    upperBound_ = newUpperBound;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ETYPED_ELEMENT__UPPER_BOUND, oldUpperBound, newUpperBound));
    // End of user code
}


bool ETypedElementImpl::isMany() const 
{
    // Start of user code ETypedElementImpl::isMany
    int upper = getUpperBound();
    return upper > 1 || upper == UNBOUNDED_MULTIPLICITY;
    // End of user code
}


bool ETypedElementImpl::isRequired() const 
{
    // Start of user code ETypedElementImpl::isRequired
    int lower = getLowerBound();
    return lower >= 1;
    // End of user code
}



//*********************************
// References
//*********************************
std::shared_ptr<ecore::EClassifier> ETypedElementImpl::getEType() const 
{
    // Start of user code ETypedElementImpl::getEType
    const auto& proxy = eType_.get(); 
    return proxy.get();
    // End of user code
}


void ETypedElementImpl::setEType(const std::shared_ptr<ecore::EClassifier>& newEType)
{
    // Start of user code ETypedElementImpl::setEType
    auto oldEType = eType_;
    eType_ = newEType;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ETYPED_ELEMENT__ETYPE, oldEType, newEType));
    // End of user code
}


void ETypedElementImpl::unsetEType()
{
    // Start of user code ETypedElementImpl::unsetEType
        if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr() , Notification::UNSET, EcorePackage::ETYPED_ELEMENT__ETYPE, boost::any(), boost::any()));
    // End of user code
}


std::shared_ptr<ecore::EGenericType> ETypedElementImpl::getEGenericType() const 
{
    // Start of user code ETypedElementImpl::getEGenericType
    return eGenericType_;
    // End of user code
}


void ETypedElementImpl::setEGenericType(const std::shared_ptr<ecore::EGenericType>& newEGenericType)
{
    // Start of user code ETypedElementImpl::setEGenericType
    if ( newEGenericType != eGenericType_ )
    {
        std::shared_ptr<ENotificationChain> notifications;
        if ( eGenericType_ )
            notifications = eGenericType_->eInverseRemove( getThisPtr(), EOPPOSITE_FEATURE_BASE - EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE , notifications );
        if ( newEGenericType )
            notifications = newEGenericType->eInverseAdd( getThisPtr(), EOPPOSITE_FEATURE_BASE - EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE , notifications );
        notifications = basicSetEGenericType(newEGenericType,notifications);
        if (notifications)
            notifications->dispatch();
    }
    // End of user code
}


void ETypedElementImpl::unsetEGenericType()
{
    // Start of user code ETypedElementImpl::unsetEGenericType
    if (eGenericType_)
    {
        std::shared_ptr<ENotificationChain> notifications;
        notifications = eGenericType_->eInverseRemove( getThisPtr(), EOPPOSITE_FEATURE_BASE - EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE , notifications );
        notifications = basicUnsetEGenericType(notifications);
        if ( notifications )
            notifications->dispatch();
    }
    else if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr() , Notification::UNSET, EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE, boost::any(), boost::any()));
    // End of user code
}



//*********************************
// Basics
//*********************************
std::shared_ptr<ecore::EClassifier> ETypedElementImpl::basicGetEType() const 
{
    // Start of user code ETypedElementImpl::basicGetEType
    const auto& proxy = eType_.get(); 
    return proxy.get();
    // End of user code
}


std::shared_ptr<ecore::ENotificationChain> ETypedElementImpl::basicSetEGenericType( const std::shared_ptr<ecore::EGenericType>& newEGenericType , const std::shared_ptr<ecore::ENotificationChain>& msgs) 
{
    // Start of user code ETypedElementImpl::basicSetEGenericType
    auto oldEGenericType = newEGenericType;
    eGenericType_ = newEGenericType;
    auto notifications = msgs;
    if ( eNotificationRequired() )
    {
        auto notification = std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE, oldEGenericType, newEGenericType );
        if (notifications)
            notifications->add( notification );
        else
            notifications = notification;
    }
    return notifications;
    // End of user code
}


std::shared_ptr<ecore::ENotificationChain> ETypedElementImpl::basicUnsetEGenericType( const std::shared_ptr<ecore::ENotificationChain>& msgs) 
{
    // Start of user code ETypedElementImpl::basicUnsetEGenericType
    auto notifications = msgs;
    if ( eNotificationRequired())
    {
        auto notification = std::make_shared<Notification>(getThisPtr() , Notification::UNSET, EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE, boost::any(), boost::any());
        if ( notifications )
            notifications->add( notification );
        else
            notifications = notification;
    }
    return notifications;
    // End of user code
}




//*********************************
// Reflectives
//*********************************
boost::any ETypedElementImpl::eGet(int featureID, bool resolve, bool coreType) const
{
    switch(featureID)
    {
        case EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE:
            return getEGenericType();
        case EcorePackage::ETYPED_ELEMENT__ETYPE:
            return getEType();
        case EcorePackage::ETYPED_ELEMENT__LOWER_BOUND:
            return getLowerBound();
        case EcorePackage::ETYPED_ELEMENT__MANY:
            return isMany();
        case EcorePackage::ETYPED_ELEMENT__ORDERED:
            return isOrdered();
        case EcorePackage::ETYPED_ELEMENT__REQUIRED:
            return isRequired();
        case EcorePackage::ETYPED_ELEMENT__UNIQUE:
            return isUnique();
        case EcorePackage::ETYPED_ELEMENT__UPPER_BOUND:
            return getUpperBound();
    }
    return ENamedElementImpl::eGet(featureID, resolve, coreType);
}


void ETypedElementImpl::eSet(int featureID, const boost::any& newValue)
{
    switch(featureID)
    {
        case EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE:
        {
            std::shared_ptr<ecore::EGenericType> e = boost::any_cast<std::shared_ptr<ecore::EGenericType>>(newValue);
            setEGenericType(e);
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__ETYPE:
        {
            std::shared_ptr<ecore::EClassifier> e = boost::any_cast<std::shared_ptr<ecore::EClassifier>>(newValue);
            setEType(e);
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__LOWER_BOUND:
        {
            int l = boost::any_cast<int>(newValue);
            setLowerBound(l);
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__ORDERED:
        {
            bool o = boost::any_cast<bool>(newValue);
            setOrdered(o);
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__UNIQUE:
        {
            bool u = boost::any_cast<bool>(newValue);
            setUnique(u);
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__UPPER_BOUND:
        {
            int u = boost::any_cast<int>(newValue);
            setUpperBound(u);
            return;
        }
    }
    return ENamedElementImpl::eSet(featureID, newValue);
}

void ETypedElementImpl::eUnset(int featureID)
{
    switch(featureID)
    {
        case EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE:
        {
            unsetEGenericType();
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__ETYPE:
        {
            unsetEType();
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__LOWER_BOUND:
        {
            setLowerBound( 0 ); 
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__ORDERED:
        {
            setOrdered( true ); 
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__UNIQUE:
        {
            setUnique( true ); 
            return;
        }
        case EcorePackage::ETYPED_ELEMENT__UPPER_BOUND:
        {
            setUpperBound( 1 ); 
            return;
        }
    }
    return ENamedElementImpl::eUnset(featureID);
}


bool ETypedElementImpl::eIsSet(int featureID) const
{
    switch(featureID)
    {
        case EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE:
            return eGenericType_ != nullptr;
        case EcorePackage::ETYPED_ELEMENT__ETYPE:
            return eType_ != nullptr;
        case EcorePackage::ETYPED_ELEMENT__LOWER_BOUND:
            return lowerBound_ != 0;
        case EcorePackage::ETYPED_ELEMENT__MANY:
            return isMany() != false;
        case EcorePackage::ETYPED_ELEMENT__ORDERED:
            return ordered_ != true;
        case EcorePackage::ETYPED_ELEMENT__REQUIRED:
            return isRequired() != false;
        case EcorePackage::ETYPED_ELEMENT__UNIQUE:
            return unique_ != true;
        case EcorePackage::ETYPED_ELEMENT__UPPER_BOUND:
            return upperBound_ != 1;
    }
    return ENamedElementImpl::eIsSet(featureID);
}

std::shared_ptr<ENotificationChain> ETypedElementImpl::eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    // Start of user code ETypedElementImpl::eBasicInverseRemove
    switch( featureID )
    {
        case EcorePackage::ETYPED_ELEMENT__EGENERIC_TYPE:
        {
            return basicUnsetEGenericType( notifications );
        }
    }
    return ENamedElementImpl::eBasicInverseRemove(otherEnd, featureID, notifications);
    // End of user code
}



//*********************************
// Initializations
//*********************************
Proxy<std::shared_ptr<ecore::EClassifier>> ETypedElementImpl::initEType()
{
    // Start of user code ETypedElementImpl::initEType
    return Proxy<std::shared_ptr<ecore::EClassifier>>( getThisPtr() , EcorePackage::ETYPED_ELEMENT__ETYPE );
    // End of user code
}




// Start of user code ETypedElementImpl [declaration-end]
// End of user code
