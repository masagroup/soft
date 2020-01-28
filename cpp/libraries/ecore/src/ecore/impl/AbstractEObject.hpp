// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTEOBJECT_HPP_
#define ECORE_ABSTRACTEOBJECT_HPP_

#include "ecore/EObject.hpp"
#include "ecore/Exports.hpp"
#include "ecore/impl/AbstractNotifier.hpp"
#include "ecore/impl/EObjectInternal.hpp"

#include <optional>

namespace ecore
{
    class ENotificationChain;
}

namespace ecore::impl
{

    template<typename... I>
    class AbstractEObject : public AbstractNotifier<I...>
    {
    public:
        AbstractEObject();
        virtual ~AbstractEObject();

        // EObjectInternal
        virtual const impl::EObjectInternal& getInternal() const;
        virtual impl::EObjectInternal& getInternal();

        // Operations
        virtual std::shared_ptr<ecore::EClass> eClass() const;
        virtual std::shared_ptr<ecore::EObject> eContainer() const;
        virtual std::shared_ptr<ecore::EStructuralFeature> eContainingFeature() const;
        virtual std::shared_ptr<ecore::EReference> eContainmentFeature() const;
        virtual std::shared_ptr<const EList<std::shared_ptr<ecore::EObject>>> eContents() const;
        virtual std::shared_ptr<const ECollectionView<std::shared_ptr<ecore::EObject>>> eAllContents() const;
        virtual std::shared_ptr<const EList<std::shared_ptr<ecore::EObject>>> eCrossReferences() const;
        virtual Any eGet(const std::shared_ptr<ecore::EStructuralFeature>& feature) const;
        virtual Any eGet(const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve) const;
        virtual Any eGet(const std::shared_ptr<ecore::EStructuralFeature>& feature, bool resolve, bool coreType) const;
        virtual bool eIsSet(const std::shared_ptr<ecore::EStructuralFeature>& feature) const;
        virtual void eSet(const std::shared_ptr<ecore::EStructuralFeature>& feature, const Any& newValue);
        virtual void eUnset(const std::shared_ptr<ecore::EStructuralFeature>& feature);
        virtual Any eInvoke(const std::shared_ptr<ecore::EOperation>& operation, const std::shared_ptr<EList<Any>>& arguments);
        virtual bool eIsProxy() const;
        virtual std::shared_ptr<EResource> eResource() const;

        // Resource
        std::shared_ptr<EResource> eDirectResource() const;

        std::shared_ptr<ENotificationChain> eSetResource(const std::shared_ptr<EResource>& resource,
            const std::shared_ptr<ENotificationChain>& notifications);

        // Container
        std::shared_ptr<ENotificationChain> eInverseAdd(const std::shared_ptr<EObject>& otherEnd,
            int featureID,
            const std::shared_ptr<ENotificationChain>& notifications);
        std::shared_ptr<ENotificationChain> eInverseRemove(const std::shared_ptr<EObject>& otherEnd,
            int featureID,
            const std::shared_ptr<ENotificationChain>& notifications);

        // Proxy
        URI eProxyURI() const;
        void eSetProxyURI(const URI& uri);
        std::shared_ptr<EObject> eResolveProxy(const std::shared_ptr<EObject>& proxy) const;



        // Fragment
        std::shared_ptr<EObject> eObjectForFragmentSegment(const std::string& uriSegment) const;
        std::string eURIFragmentSegment(const std::shared_ptr<EStructuralFeature>& feature,
            const std::shared_ptr<EObject>& eObject) const;

    protected:
        template <typename U>
        class EObjectInternalAdapter;

        virtual std::unique_ptr<EObjectInternal> createInternal();

        virtual std::shared_ptr<EClass> eStaticClass() const;
        virtual int eDerivedStructuralFeatureID(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const;
        virtual int eDerivedOperationID(const std::shared_ptr<EOperation>& eOperation) const;

        virtual Any eGet(int featureID, bool resolve, bool coreType) const;
        virtual bool eIsSet(int featureID) const;
        virtual void eSet(int featureID, const Any& newValue);
        virtual void eUnset(int featureID);
        virtual Any eInvoke(int operationID, const std::shared_ptr<EList<Any>>& arguments);

        virtual std::shared_ptr<ENotificationChain> eBasicInverseAdd(const std::shared_ptr<EObject>& otherEnd,
            int featureID,
            const std::shared_ptr<ENotificationChain>& notifications);
        virtual std::shared_ptr<ENotificationChain> eBasicInverseRemove(const std::shared_ptr<EObject>& otherEnd,
            int featureID,
            const std::shared_ptr<ENotificationChain>& notifications);
        std::shared_ptr<ENotificationChain> eBasicSetContainer(const std::shared_ptr<EObject>& newContainer,
            int newContainerFeatureID,
            const std::shared_ptr<ENotificationChain>& notifications);
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainer(const std::shared_ptr<ENotificationChain>& notifications);
        std::shared_ptr<ENotificationChain> eBasicRemoveFromContainerFeature(const std::shared_ptr<ENotificationChain>& notifications);
        static std::shared_ptr<EReference> eContainmentFeature(const std::shared_ptr<EObject>& eObject,
            const std::shared_ptr<EObject>& eContainer,
            int eContainerFeatureID);

        void eSetDirectResource(const std::shared_ptr<EResource>& resource);

        int eContainerFeatureID() const;

    private:
        std::shared_ptr<const EList<std::shared_ptr<EObject>>> eContentsList(
            const std::shared_ptr<const EList<std::shared_ptr<ecore::EReference>>>& refs) const;
        std::shared_ptr<EStructuralFeature> eStructuralFeature(const std::string& name) const;

    protected:
        std::unique_ptr<EObjectInternal> internal_;
        std::weak_ptr<EResource> eResource_;
        std::weak_ptr<EObject> eContainer_;
        int eContainerFeatureID_;
        std::optional<URI> eProxyURI_;
    };

} // namespace ecore::impl

#include "ecore/impl/AbstractEObject.inl"

#endif /* ECORE_ABSTRACTEOBJECT_HPP_ */
