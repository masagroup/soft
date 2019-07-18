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

#include "ecore/Exports.hpp"
#include "ecore/EObject.hpp"
#include "ecore/impl/AbstractNotifier.hpp"

#include <optional>

namespace ecore
{
    class ENotificationChain;
}

namespace ecore::impl
{

    class ECORE_API BasicEObject : public virtual AbstractNotifier<EObject>
    {
    public:
        BasicEObject();
        virtual ~BasicEObject();

        // Operations
        virtual std::shared_ptr<ecore::EClass> eClass() const;
        virtual std::shared_ptr<ecore::EObject> eContainer() const;
        virtual std::shared_ptr<ecore::EStructuralFeature> eContainingFeature() const;
        virtual std::shared_ptr<ecore::EReference> eContainmentFeature() const;
        virtual std::shared_ptr<const EList<std::shared_ptr<ecore::EObject>>> eContents() const;
        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<ecore::EObject>>> eAllContents() const;
        virtual std::shared_ptr<const EList<std::shared_ptr<ecore::EObject>>> eCrossReferences() const;
        virtual Any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature ) const;
        virtual Any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve ) const;
        virtual Any eGet( const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve, bool coreType ) const;
        virtual bool eIsSet( const std::shared_ptr<ecore::EStructuralFeature>& feature ) const;
        virtual void eSet( const std::shared_ptr<ecore::EStructuralFeature>& feature, const Any& newValue );
        virtual void eUnset( const std::shared_ptr<ecore::EStructuralFeature>& feature );
        virtual Any eInvoke( const std::shared_ptr<ecore::EOperation>& operation, const std::shared_ptr<EList<Any>>& arguments );
        virtual bool eIsProxy() const;
        virtual std::shared_ptr<EResource> eResource() const;

        // Resource
        virtual std::shared_ptr<ENotificationChain> eSetResource( const std::shared_ptr<EResource>& resource,
                                                                  const std::shared_ptr<ENotificationChain>& notifications );

        // Container
        std::shared_ptr<ENotificationChain> eInverseAdd( const std::shared_ptr<EObject>& otherEnd,
                                                         int featureID,
                                                         const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eInverseRemove( const std::shared_ptr<EObject>& otherEnd,
                                                            int featureID,
                                                            const std::shared_ptr<ENotificationChain>& notifications );

        // Proxy
        virtual Uri eProxyUri() const;
        virtual void eSetProxyURI( const Uri& uri );
        virtual std::shared_ptr<EObject> eResolveProxy( const std::shared_ptr<EObject>& proxy ) const;

        int eContainerFeatureID() const;

    protected:
        virtual std::shared_ptr<EClass> eStaticClass() const;
        virtual int eDerivedStructuralFeatureID( const std::shared_ptr<EStructuralFeature>& eStructuralFeature ) const;
        virtual int eDerivedOperationID( const std::shared_ptr<EOperation>& eOperation ) const;

        virtual Any eGet( int featureID, bool resolve, bool coreType ) const;
        virtual bool eIsSet( int featureID ) const;
        virtual void eSet( int featureID, const Any& newValue );
        virtual void eUnset( int featureID );
        virtual Any eInvoke( int operationID, const std::shared_ptr<EList<Any>>& arguments );

        virtual std::shared_ptr<ENotificationChain> eBasicInverseAdd( const std::shared_ptr<EObject>& otherEnd,
                                                                      int featureID,
                                                                      const std::shared_ptr<ENotificationChain>& notifications );
        virtual std::shared_ptr<ENotificationChain> eBasicInverseRemove( const std::shared_ptr<EObject>& otherEnd,
                                                                         int featureID,
                                                                         const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicSetContainer( const std::shared_ptr<EObject>& newContainer,
                                                                int newContainerFeatureID,
                                                                const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainer( const std::shared_ptr<ENotificationChain>& notifications );
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainerFeature( const std::shared_ptr<ENotificationChain>& notifications );
        static std::shared_ptr<EReference> eContainmentFeature( const std::shared_ptr<EObject>& eObject,
                                                                const std::shared_ptr<EObject>& eContainer,
                                                                int eContainerFeatureID );

    protected:
        std::weak_ptr<EResource> eResource_;
        std::weak_ptr<EObject> eContainer_;
        int eContainerFeatureID_;
        std::optional<Uri> eProxyUri_;
    };

    template <typename T>
    T objectCast( const Any& any )
    {
        auto id = &any.type();
        if( id == &typeid( T ) )
            return anyCast<T>( any );
        else if( id == &typeid( std::shared_ptr<EObject> ) )
        {
            auto object = anyCast<std::shared_ptr<EObject>>( any );
            return std::dynamic_pointer_cast<typename T::element_type>( object );
        }
        return nullptr;
    }

} // namespace ecore::impl

#endif /* ECORE_BASICEOBJECT_HPP_ */