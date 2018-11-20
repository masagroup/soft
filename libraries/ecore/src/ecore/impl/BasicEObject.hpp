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
}

namespace ecore::impl
{

    class BasicEObject : public virtual EObject
    {
    public:
        BasicEObject();
        virtual ~BasicEObject();

        void setThisPtr( const std::shared_ptr<BasicEObject>& thisPtr );
        std::shared_ptr<BasicEObject> getThisPtr() const;

        // Notification
        virtual EList<EAdapter*>& eAdapters() const;
        virtual bool eDeliver() const;
        virtual void eSetDeliver( bool deliver );
        virtual void eNotify( const std::shared_ptr<ENotification>& notification );
        bool eNotificationRequired();

        // Operations
        virtual std::shared_ptr<EList<std::shared_ptr<ecore::EObject>>> eAllContents() const;
        virtual std::shared_ptr<ecore::EClass> eClass() const;
        virtual std::shared_ptr<ecore::EObject> eContainer() const;
        virtual std::shared_ptr<ecore::EStructuralFeature> eContainingFeature() const;
        virtual std::shared_ptr<ecore::EReference> eContainmentFeature() const;
        virtual std::shared_ptr<EList<std::shared_ptr<ecore::EObject>>> eContents() const;
        virtual std::shared_ptr<EList<std::shared_ptr<ecore::EObject>>> eCrossReferences() const;
        virtual boost::any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature ) const;
        virtual boost::any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve ) const;
        virtual boost::any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve, bool coreType ) const;
        virtual bool eIsSet( const std::shared_ptr<ecore::EStructuralFeature>& feature ) const;
        virtual void eSet( const std::shared_ptr<ecore::EStructuralFeature>& feature, const boost::any& newValue );
        virtual void eUnset( const std::shared_ptr<ecore::EStructuralFeature>& feature );
        virtual boost::any eInvoke( const std::shared_ptr<ecore::EOperation>& operation, const std::shared_ptr<EList<boost::any>>& arguments );
        virtual bool eIsProxy() const;
        virtual int eResource() const;

        int eContainerFeatureID() const;

        // Container
        std::shared_ptr<ENotificationChain> eInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID , const std::shared_ptr<ENotificationChain>& notifications  );
        std::shared_ptr<ENotificationChain> eInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );

    protected:
        virtual std::shared_ptr<EClass> eStaticClass() const;
        virtual int eDerivedStructuralFeatureID( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const;
        virtual int eDerivedOperationID( const std::shared_ptr<EOperation>& eOperation ) const;

        virtual boost::any eGet( int featureID, bool resolve, bool coreType ) const;
        virtual bool eIsSet( int featureID ) const;
        virtual void eSet( int featureID, const boost::any& newValue );
        virtual void eUnset( int featureID );
        virtual boost::any eInvoke( int operationID, const std::shared_ptr<EList<boost::any>>& arguments );

        virtual std::shared_ptr<ENotificationChain> eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );
        virtual std::shared_ptr<ENotificationChain> eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd, int featureID, const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicSetContainer( const std::shared_ptr<EObject>& newContainer, int newContainerFeatureID, const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainer( const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainerFeature( const std::shared_ptr<ENotificationChain>& notifications );
        static std::shared_ptr<EReference> eContainmentFeature( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EObject>& eContainer, int eContainerFeatureID );
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
