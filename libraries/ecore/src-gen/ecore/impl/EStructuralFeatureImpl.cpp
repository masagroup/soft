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
#include "ecore/impl/EStructuralFeatureImpl.hpp"
#include "ecore/EAnnotation.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/EGenericType.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/ETypedElement.hpp"
#include "ecore/EcoreFactory.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/impl/EObjectEList.hpp"


#include <cassert>
#include <iostream>
#include <sstream>

//Start of user code EStructuralFeatureImpl [declaration-includes]
#include "ecore/impl/EClassImpl.hpp"
//End of user code


using namespace ecore;
using namespace ecore::impl;

//Start of user code EStructuralFeatureImpl [declaration-begin]
//End of user code

//*********************************
// Constructor / Destructor
//*********************************
EStructuralFeatureImpl::EStructuralFeatureImpl()
:   changeable_(true)
,   volatile_(false)
,   transient_(false)
,   defaultValueLiteral_("")
,   unsettable_(false)
,   derived_(false)
,   eContainingClass_()
,   featureID_(-1)
//Start of user code EStructuralFeatureImpl::EStructuralFeatureImpl
{
}
//End of user code


EStructuralFeatureImpl::~EStructuralFeatureImpl()
{
//Start of user code EStructuralFeatureImpl::~EStructuralFeatureImpl
//End of user code
}


std::shared_ptr<EClass> EStructuralFeatureImpl::eStaticClass() const
{
    return EcorePackage::eInstance()->getEStructuralFeature();
}

std::shared_ptr<EStructuralFeatureImpl> EStructuralFeatureImpl::getThisPtr() const
{
    return thisPtr_.lock();
}

void EStructuralFeatureImpl::setThisPtr(const std::shared_ptr<EStructuralFeatureImpl>& thisPtr)
{
    thisPtr_ = thisPtr;
    ETypedElementImpl::setThisPtr(thisPtr);
}

//*********************************
// Operations
//*********************************
void * EStructuralFeatureImpl::getContainerClass()
{
    // Start of user code EStructuralFeatureImpl::getContainerClass
    std::cout << BOOST_CURRENT_FUNCTION  << std::endl;
    throw "NotImplementedException";
    // End of user code
}

//*********************************
// Attributes
//*********************************
bool EStructuralFeatureImpl::isChangeable() const 
{
    // Start of user code EStructuralFeatureImpl::isChangeable
    return changeable_;
    // End of user code
}


void EStructuralFeatureImpl::setChangeable(bool newChangeable)
{
    // Start of user code EStructuralFeatureImpl::setChangeable
    auto oldChangeable = changeable_;
    changeable_ = newChangeable;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ESTRUCTURAL_FEATURE__CHANGEABLE, oldChangeable, newChangeable));
    // End of user code
}


bool EStructuralFeatureImpl::isVolatile() const 
{
    // Start of user code EStructuralFeatureImpl::isVolatile
    return volatile_;
    // End of user code
}


void EStructuralFeatureImpl::setVolatile(bool newVolatile)
{
    // Start of user code EStructuralFeatureImpl::setVolatile
    auto oldVolatile = volatile_;
    volatile_ = newVolatile;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ESTRUCTURAL_FEATURE__VOLATILE, oldVolatile, newVolatile));
    // End of user code
}


bool EStructuralFeatureImpl::isTransient() const 
{
    // Start of user code EStructuralFeatureImpl::isTransient
    return transient_;
    // End of user code
}


void EStructuralFeatureImpl::setTransient(bool newTransient)
{
    // Start of user code EStructuralFeatureImpl::setTransient
    auto oldTransient = transient_;
    transient_ = newTransient;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ESTRUCTURAL_FEATURE__TRANSIENT, oldTransient, newTransient));
    // End of user code
}


const std::string& EStructuralFeatureImpl::getDefaultValueLiteral() const 
{
    // Start of user code EStructuralFeatureImpl::getDefaultValueLiteral
    return defaultValueLiteral_;
    // End of user code
}


void EStructuralFeatureImpl::setDefaultValueLiteral(const std::string& newDefaultValueLiteral)
{
    // Start of user code EStructuralFeatureImpl::setDefaultValueLiteral
    auto oldDefaultValueLiteral = defaultValueLiteral_;
    defaultValueLiteral_ = newDefaultValueLiteral;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE_LITERAL, oldDefaultValueLiteral, newDefaultValueLiteral));
    // End of user code
}


const boost::any& EStructuralFeatureImpl::getDefaultValue() const 
{
    // Start of user code EStructuralFeatureImpl::getDefaultValue
    std::cout << BOOST_CURRENT_FUNCTION  << std::endl;
    throw "NotImplementedException";
    // End of user code
}


void EStructuralFeatureImpl::setDefaultValue(const boost::any& newDefaultValue)
{
    // Start of user code EStructuralFeatureImpl::setDefaultValue
    std::cout << BOOST_CURRENT_FUNCTION  << std::endl;
    throw "NotImplementedException";    
    // End of user code
}


bool EStructuralFeatureImpl::isUnsettable() const 
{
    // Start of user code EStructuralFeatureImpl::isUnsettable
    return unsettable_;
    // End of user code
}


void EStructuralFeatureImpl::setUnsettable(bool newUnsettable)
{
    // Start of user code EStructuralFeatureImpl::setUnsettable
    auto oldUnsettable = unsettable_;
    unsettable_ = newUnsettable;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ESTRUCTURAL_FEATURE__UNSETTABLE, oldUnsettable, newUnsettable));
    // End of user code
}


bool EStructuralFeatureImpl::isDerived() const 
{
    // Start of user code EStructuralFeatureImpl::isDerived
    return derived_;
    // End of user code
}


void EStructuralFeatureImpl::setDerived(bool newDerived)
{
    // Start of user code EStructuralFeatureImpl::setDerived
    auto oldDerived = derived_;
    derived_ = newDerived;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ESTRUCTURAL_FEATURE__DERIVED, oldDerived, newDerived));
    // End of user code
}


int EStructuralFeatureImpl::getFeatureID() const 
{
    // Start of user code EStructuralFeatureImpl::getFeatureID
    return featureID_;
    // End of user code
}


void EStructuralFeatureImpl::setFeatureID(int newFeatureID)
{
    // Start of user code EStructuralFeatureImpl::setFeatureID
    auto oldFeatureID = featureID_;
    featureID_ = newFeatureID;
    if ( eNotificationRequired() )
        eNotify( std::make_shared<Notification>(getThisPtr(), Notification::SET, EcorePackage::ESTRUCTURAL_FEATURE__FEATURE_ID, oldFeatureID, newFeatureID));
    // End of user code
}



//*********************************
// References
//*********************************
std::shared_ptr<ecore::EClass> EStructuralFeatureImpl::getEContainingClass() const 
{
    // Start of user code EStructuralFeatureImpl::getEContainingClass
    if ( eContainerFeatureID() != EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS )
        return std::shared_ptr<ecore::EClass>();
    else
        return std::dynamic_pointer_cast<ecore::EClass>(eContainer());
    // End of user code
}




//*********************************
// Reflectives
//*********************************
boost::any EStructuralFeatureImpl::eGet(int featureID, bool resolve, bool coreType) const
{
    switch(featureID)
    {
        case EcorePackage::ESTRUCTURAL_FEATURE__CHANGEABLE:
            return isChangeable();
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE:
            return getDefaultValue();
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE_LITERAL:
            return getDefaultValueLiteral();
        case EcorePackage::ESTRUCTURAL_FEATURE__DERIVED:
            return isDerived();
        case EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS:
            return getEContainingClass();
        case EcorePackage::ESTRUCTURAL_FEATURE__FEATURE_ID:
            return getFeatureID();
        case EcorePackage::ESTRUCTURAL_FEATURE__TRANSIENT:
            return isTransient();
        case EcorePackage::ESTRUCTURAL_FEATURE__UNSETTABLE:
            return isUnsettable();
        case EcorePackage::ESTRUCTURAL_FEATURE__VOLATILE:
            return isVolatile();
    }
    return ETypedElementImpl::eGet(featureID, resolve, coreType);
}


void EStructuralFeatureImpl::eSet(int featureID, const boost::any& newValue)
{
    switch(featureID)
    {
        case EcorePackage::ESTRUCTURAL_FEATURE__CHANGEABLE:
        {
            bool c = boost::any_cast<bool>(newValue);
            setChangeable(c);
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE:
        {
            boost::any d = boost::any_cast<boost::any>(newValue);
            setDefaultValue(d);
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE_LITERAL:
        {
            std::string d = boost::any_cast<std::string>(newValue);
            setDefaultValueLiteral(d);
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__DERIVED:
        {
            bool d = boost::any_cast<bool>(newValue);
            setDerived(d);
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__FEATURE_ID:
        {
            int f = boost::any_cast<int>(newValue);
            setFeatureID(f);
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__TRANSIENT:
        {
            bool t = boost::any_cast<bool>(newValue);
            setTransient(t);
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__UNSETTABLE:
        {
            bool u = boost::any_cast<bool>(newValue);
            setUnsettable(u);
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__VOLATILE:
        {
            bool v = boost::any_cast<bool>(newValue);
            setVolatile(v);
            return;
        }
    }
    return ETypedElementImpl::eSet(featureID, newValue);
}

void EStructuralFeatureImpl::eUnset(int featureID)
{
    switch(featureID)
    {
        case EcorePackage::ESTRUCTURAL_FEATURE__CHANGEABLE:
        {
            setChangeable( true ); 
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE:
        {
            setDefaultValue( nullptr ); 
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE_LITERAL:
        {
            setDefaultValueLiteral( "" ); 
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__DERIVED:
        {
            setDerived( false ); 
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__FEATURE_ID:
        {
            setFeatureID( -1 ); 
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__TRANSIENT:
        {
            setTransient( false ); 
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__UNSETTABLE:
        {
            setUnsettable( false ); 
            return;
        }
        case EcorePackage::ESTRUCTURAL_FEATURE__VOLATILE:
        {
            setVolatile( false ); 
            return;
        }
    }
    return ETypedElementImpl::eUnset(featureID);
}


template <typename T>
bool is_uninitialized(std::weak_ptr<T> const& weak) {
    using wt = std::weak_ptr<T>;
    return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
}

bool EStructuralFeatureImpl::eIsSet(int featureID) const
{
    switch(featureID)
    {
        case EcorePackage::ESTRUCTURAL_FEATURE__CHANGEABLE:
            return changeable_ != true;
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE:
            return !getDefaultValue().empty();
        case EcorePackage::ESTRUCTURAL_FEATURE__DEFAULT_VALUE_LITERAL:
            return defaultValueLiteral_ != "";
        case EcorePackage::ESTRUCTURAL_FEATURE__DERIVED:
            return derived_ != false;
        case EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS:
            return !is_uninitialized(eContainingClass_);
        case EcorePackage::ESTRUCTURAL_FEATURE__FEATURE_ID:
            return featureID_ != -1;
        case EcorePackage::ESTRUCTURAL_FEATURE__TRANSIENT:
            return transient_ != false;
        case EcorePackage::ESTRUCTURAL_FEATURE__UNSETTABLE:
            return unsettable_ != false;
        case EcorePackage::ESTRUCTURAL_FEATURE__VOLATILE:
            return volatile_ != false;
    }
    return ETypedElementImpl::eIsSet(featureID);
}

boost::any EStructuralFeatureImpl::eInvoke(int operationID, const std::shared_ptr<EList<boost::any>>& arguments)
{
    // Start of user code EStructuralFeatureImpl::eInvoke
    switch( operationID )
    {
        case EcorePackage::ESTRUCTURAL_FEATURE__GET_CONTAINER_CLASS:
        {
            return getContainerClass(); 
        }
    }
    return boost::any();
    // End of user code
}


std::shared_ptr<ENotificationChain> EStructuralFeatureImpl::eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    // Start of user code EStructuralFeatureImpl::eBasicInverseAdd
    switch( featureID )
    {
        case EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS:
        {
            auto msgs = notifications;
            if (eContainer())
                msgs = eBasicRemoveFromContainer(msgs);
            return eBasicSetContainer( otherEnd , EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS, msgs );
        }
    }
    return ETypedElementImpl::eBasicInverseAdd(otherEnd, featureID, notifications);
    // End of user code
}


std::shared_ptr<ENotificationChain> EStructuralFeatureImpl::eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications )
{
    // Start of user code EStructuralFeatureImpl::eBasicInverseRemove
    switch( featureID )
    {
        case EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS:
        {
            return eBasicSetContainer( nullptr , EcorePackage::ESTRUCTURAL_FEATURE__ECONTAINING_CLASS, notifications );
        }
    }
    return ETypedElementImpl::eBasicInverseRemove(otherEnd, featureID, notifications);
    // End of user code
}





// Start of user code EStructuralFeatureImpl [declaration-end]
void EStructuralFeatureImpl::setName( const std::string & newName )
{
    ETypedElementImpl::setName( newName );

    auto eContainingClass = std::dynamic_pointer_cast<EClassImpl>(eContainingClass_.lock());
    if (eContainingClass)
        eContainingClass->setModified( EcorePackage::ECLASS__ESTRUCTURAL_FEATURES );
}
// End of user code

