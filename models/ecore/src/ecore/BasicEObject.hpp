// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_BASICEOBJECT_HPP_
#define ECORE_BASICEOBJECT_HPP_

#include "ecore/EObject.hpp"

namespace ecore
{
    class ENotificationChain;

    class BasicEObject : public virtual ecore::EObject
    {
    public:
        static const int EOPPOSITE_FEATURE_BASE = -1;

    protected:
        BasicEObject();

    public:
        virtual ~BasicEObject();

        void setThisPtr( const std::shared_ptr<BasicEObject>& thisPtr );
        std::shared_ptr<BasicEObject> getThisPtr() const;

        // Notification
        virtual EList<EAdapter*>& eAdapters();
        virtual bool eDeliver();
        virtual void eSetDeliver( bool deliver );
        virtual void eNotify( const std::shared_ptr<ENotification>& notification );
        bool eNotificationRequired();

        // Getter/ Setter
        virtual boost::any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature ) const;
        virtual boost::any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve ) const;
        virtual boost::any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve, bool coreType ) const;
        virtual bool eIsSet( const std::shared_ptr<ecore::EStructuralFeature>& feature ) const;
        virtual void eSet( const std::shared_ptr<ecore::EStructuralFeature>& feature, const boost::any& newValue );
        virtual void eUnset( const std::shared_ptr<ecore::EStructuralFeature>& feature );
        virtual boost::any eInvoke( const std::shared_ptr<ecore::EOperation>& operation, const std::shared_ptr<EList<boost::any>>& arguments );

        // Container
        std::shared_ptr<ENotificationChain> eInverseAdd( const std::shared_ptr<BasicEObject>& otherEnd, int featureID , const std::shared_ptr<ENotificationChain>& notifications  );
        std::shared_ptr<ENotificationChain> eInverseRemove( const std::shared_ptr<BasicEObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );

    protected:
        virtual int eDerivedStructuralFeatureID( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const;
        virtual int eDerivedOperationID( const std::shared_ptr<EOperation>& eOperation ) const;

        virtual boost::any eGet( int featureID, bool resolve, bool coreType ) const;
        virtual bool eIsSet( int featureID ) const;
        virtual void eSet( int featureID, const boost::any& newValue );
        virtual void eUnset( int featureID );
        virtual boost::any eInvoke( int operationID, const std::shared_ptr<EList<boost::any>>& arguments );

        virtual std::shared_ptr<ENotificationChain> eBasicInverseAdd( const std::shared_ptr<BasicEObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );
        virtual std::shared_ptr<ENotificationChain> eBasicInverseRemove( const std::shared_ptr<BasicEObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicSetContainer( const std::shared_ptr<EObject>& newContainer, int newContainerFeatureID, const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainer( const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainerFeature( const std::shared_ptr<ENotificationChain>& notifications );

    protected:
        std::shared_ptr< EList<EAdapter*> > eAdapters_;
        bool eDeliver_;
        std::weak_ptr<EObject> eContainer_;
        int eContainerFeatureID_;

    private:
        std::weak_ptr<BasicEObject> thisPtr_;
    };

}

#endif /* ECORE_BASICEOBJECT_HPP_ */
