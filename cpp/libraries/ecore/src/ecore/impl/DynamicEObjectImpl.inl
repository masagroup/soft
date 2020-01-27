// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTEOBJECT_HPP_
#error This file may only be included from DynamicEObjectBase.hpp
#endif

#include "ecore/EClass.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EList.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EReference.hpp"
#include "ecore/Stream.hpp"
#include "ecore/impl/AbstractAdapter.hpp"
#include "ecore/impl/ArrayEList.hpp"
#include "ecore/impl/EObjectEList.hpp"
#include "ecore/impl/Proxy.hpp"
#include "ecore/impl/WeakPtr.hpp"

#include <string>
#include <sstream>

namespace std
{
    template <typename T>
    bool operator ==(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs)
    {
        return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
    }
}

namespace ecore::impl
{

    using EObjectProxy = Proxy< std::shared_ptr<EObject> >;

    template <typename... I>
    class DynamicEObjectBase<I...>::FeaturesAdapter : public AbstractAdapter
    {
    public:
        FeaturesAdapter(DynamicEObjectBase& eObject)
            : eObject_(eObject)
        {
        }

        virtual ~FeaturesAdapter()
        {

        }

        virtual void notifyChanged(const std::shared_ptr<ENotification>& notification)
        {
            int eventType = notification->getEventType();
            auto eNotifier = std::dynamic_pointer_cast<DynamicEObjectBase>(notification->getNotifier());
            if (eventType != ENotification::REMOVING_ADAPTER)
            {
                int featureID = notification->getFeatureID();
                if (featureID == EcorePackage::ECLASS__ESTRUCTURAL_FEATURES)
                    eObject_.resizeProperties();
            }
        }

    private:
        DynamicEObjectBase& eObject_;
    };

    template <typename... I>
    DynamicEObjectBase<I...>::DynamicEObjectBase()
        : featuresAdapter_(new FeaturesAdapter(*this))
    {
    }

    template <typename... I>
    DynamicEObjectBase<I...>::DynamicEObjectBase(const std::shared_ptr<EClass>& eClass)
        : featuresAdapter_(new FeaturesAdapter(*this))
    {
        setEClass(eClass);
    }

    template <typename... I>
    DynamicEObjectBase<I...>::~DynamicEObjectBase()
    {
        setEClass(nullptr);
    }

    template <typename... I>
    std::shared_ptr<ecore::EClass> DynamicEObjectBase<I...>::eClass() const
    {
        return is_uninitialized(eClass_) ? eStaticClass() : eClass_.lock();
    }

    template <typename... I>
    void DynamicEObjectBase<I...>::setEClass(const std::shared_ptr<EClass>& newClass)
    {
        if (auto eClass = eClass_.lock())
            eClass->eAdapters().remove(featuresAdapter_.get());

        eClass_ = newClass;
        resizeProperties();

        if (auto eClass = eClass_.lock())
            eClass->eAdapters().add(featuresAdapter_.get());
    }

    template <typename... I>
    std::shared_ptr<DynamicEObjectBase<I...>> DynamicEObjectBase<I...>::getThisPtr() const
    {
        return std::static_pointer_cast<DynamicEObjectBase>(EObjectBase<I...>::getThisPtr());
    }

    template <typename... I>
    void DynamicEObjectBase<I...>::setThisPtr(const std::shared_ptr<DynamicEObjectBase<I...>>& thisPtr)
    {
        EObjectBase<I...>::setThisPtr(thisPtr);
    }

    template <typename... I>
    Any DynamicEObjectBase<I...>::eGet(int featureID, bool resolve, bool coreType) const
    {
        int dynamicFeatureID = featureID - eStaticFeatureCount();
        if (dynamicFeatureID >= 0)
        {
            auto eFeature = eDynamicFeature(featureID);

            // retrieve value or compute it if empty
            auto result = properties_[dynamicFeatureID];
            if (result.empty())
            {
                if (eFeature->isMany())
                    properties_[dynamicFeatureID] = result = createList(eFeature);
                else if (isProxy(eFeature))
                    properties_[dynamicFeatureID] = result = std::make_shared<EObjectProxy>();
                else if (isBackReference(eFeature))
                    properties_[dynamicFeatureID] = result = std::weak_ptr<EObject>();
            }

            // convert internal value to ouput value
            if (isProxy(eFeature))
            {
                auto proxy = anyCast<std::shared_ptr<EObjectProxy>>(result);
                result = proxy->get();
            }
            else if (isBackReference(eFeature))
            {
                auto weak = anyCast<std::weak_ptr<EObject>>(result);
                result = weak.lock();
            }

            return result;
        }
        return EObjectBase<I...>::eGet(featureID, resolve, coreType);
    }

    template <typename... I>
    bool DynamicEObjectBase<I...>::eIsSet(int featureID) const
    {
        int dynamicFeatureID = featureID - eStaticFeatureCount();
        if (dynamicFeatureID >= 0)
            return !properties_[dynamicFeatureID].empty();
        else
            return EObjectBase<I...>::eIsSet(featureID);
    }

    template <typename... I>
    void DynamicEObjectBase<I...>::eSet(int featureID, const Any& newValue)
    {
        int dynamicFeatureID = featureID - eStaticFeatureCount();
        if (dynamicFeatureID >= 0)
        {
            auto dynamicFeature = eDynamicFeature(featureID);
            if (isContainer(dynamicFeature))
            {
                ASSERT(newValue.type() == typeid(std::shared_ptr<EObject>()), " Feature defined as a container : value must be a std::shared_ptr<EObject>");

                // container
                std::shared_ptr<EObject> newContainer = anyCast<std::shared_ptr<EObject>>(newValue);
                if (newContainer != eContainer() || (newContainer && eContainerFeatureID() != featureID))
                {
                    std::shared_ptr<ENotificationChain> notifications;
                    if (eContainer())
                        notifications = eBasicRemoveFromContainer(notifications);
                    if (newContainer)
                        notifications = newContainer->getInternal().eInverseAdd(getThisPtr(), featureID, notifications);
                    notifications = eBasicSetContainer(newContainer, featureID, notifications);
                    if (notifications)
                        notifications->dispatch();
                }
                else if (eNotificationRequired())
                    eNotify(std::make_shared<Notification>(getThisPtr(), Notification::SET, featureID, newValue, newValue));
            }
            else if (isBidirectional(dynamicFeature) || isContains(dynamicFeature))
            {
                ASSERT(newValue.type() == typeid(std::shared_ptr<EObject>()), " Feature defined as birectional or containment : value must be a std::shared_ptr<EObject>");

                // inverse - opposite
                auto oldValue = properties_[dynamicFeatureID];
                if (oldValue != newValue)
                {
                    std::shared_ptr<ENotificationChain> notifications;
                    std::shared_ptr<EObject> oldObject;
                    if (isProxy(dynamicFeature))
                        oldObject = anyCast<std::shared_ptr<EObjectProxy>>(oldValue)->get();
                    else if (isBackReference(dynamicFeature))
                        oldObject = anyCast<std::weak_ptr<EObject>>(oldValue).lock();
                    else
                        oldObject = anyCast<std::shared_ptr<EObject>>(oldValue);

                    auto newObject = anyCast<std::shared_ptr<EObject>>(newValue);

                    if (!isBidirectional(dynamicFeature))
                    {
                        if (oldObject)
                            notifications = oldObject->getInternal().eInverseRemove(getThisPtr(), EOPPOSITE_FEATURE_BASE - featureID, notifications);

                        if (newObject)
                            notifications = newObject->getInternal().eInverseAdd(getThisPtr(), EOPPOSITE_FEATURE_BASE - featureID, notifications);
                    }
                    else
                    {
                        auto dynamicReference = std::dynamic_pointer_cast<EReference>(dynamicFeature);
                        auto reverseFeature = dynamicReference->getEOpposite();
                        if (oldObject)
                            notifications = oldObject->getInternal().eInverseRemove(getThisPtr(), reverseFeature->getFeatureID(), notifications);

                        if (newObject)
                            notifications = newObject->getInternal().eInverseAdd(getThisPtr(), reverseFeature->getFeatureID(), notifications);
                    }
                    // basic set
                    if (isProxy(dynamicFeature))
                    {
                        auto proxy = anyCast < std::shared_ptr<EObjectProxy> >(oldValue);
                        proxy->set(newObject);
                    }
                    else if (isBackReference(dynamicFeature))
                    {
                        properties_[dynamicFeatureID] = std::weak_ptr<EObject>(newObject);
                    }
                    else
                    {
                        properties_[dynamicFeatureID] = newValue;
                    }

                    // create notification
                    if (eNotificationRequired())
                    {
                        auto notification = std::make_shared<Notification>(getThisPtr(), Notification::SET, featureID, oldValue, newValue);
                        if (notifications)
                            notifications->add(notification);
                        else
                            notifications = notification;
                    }

                    // notify
                    if (notifications)
                        notifications->dispatch();
                }

            }
            else
            {
                // basic set
                auto oldValue = properties_[dynamicFeatureID];

                if (isProxy(dynamicFeature))
                {
                    ASSERT(newValue.type() == typeid(std::shared_ptr<EObject>()), " Feature defined as reference proxy : value must be a std::shared_ptr<EObject>");
                    auto newObject = anyCast<std::shared_ptr<EObject>>(newValue);

                    auto proxy = anyCast < std::shared_ptr<EObjectProxy> >(oldValue);
                    proxy->set(newObject);
                }
                else if (isBackReference(dynamicFeature))
                {
                    ASSERT(newValue.type() == typeid(std::shared_ptr<EObject>()), " Feature defined as a back reference : value must be a std::shared_ptr<EObject>");
                    auto newObject = anyCast<std::shared_ptr<EObject>>(newValue);

                    properties_[dynamicFeatureID] = std::weak_ptr<EObject>(newObject);
                }
                else
                    properties_[dynamicFeatureID] = newValue;

                // notify
                if (eNotificationRequired())
                    eNotify(std::make_shared<Notification>(getThisPtr(), Notification::SET, featureID, oldValue, newValue));
            }
        }
        else
            EObjectBase<I...>::eSet(featureID, newValue);
    }

    template <typename... I>
    void DynamicEObjectBase<I...>::eUnset(int featureID)
    {
        int dynamicFeatureID = featureID - eStaticFeatureCount();
        if (dynamicFeatureID >= 0)
        {
            auto oldValue = properties_[dynamicFeatureID];

            properties_[dynamicFeatureID].reset();

            if (eNotificationRequired())
                eNotify(std::make_shared<Notification>(getThisPtr(), Notification::UNSET, featureID, oldValue, NO_VALUE));
        }
        else
            EObjectBase<I...>::eUnset(featureID);
    }

    template <typename... I>
    Any DynamicEObjectBase<I...>::eInvoke(int operationID, const std::shared_ptr<EList<Any>>& arguments)
    {
        return Any();
    }

    template <typename... I>
    int DynamicEObjectBase<I...>::eStaticFeatureCount() const
    {
        return eStaticClass()->getFeatureCount();
    }

    template <typename... I>
    int DynamicEObjectBase<I...>::eStaticOperationCount() const
    {
        return eStaticClass()->getOperationCount();
    }

    template <typename... I>
    int DynamicEObjectBase<I...>::eDynamicFeatureID(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const
    {
        return eClass()->getFeatureID(eStructuralFeature) - eStaticFeatureCount();
    }

    template <typename... I>
    std::shared_ptr<EStructuralFeature> DynamicEObjectBase<I...>::eDynamicFeature(int dynamicFeatureID) const
    {
        return eClass()->getEStructuralFeature(dynamicFeatureID + eStaticFeatureCount());
    }

    template <typename... I>
    bool DynamicEObjectBase<I...>::isBidirectional(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const
    {
        if (auto eReference = std::dynamic_pointer_cast<EReference>(eStructuralFeature))
            return static_cast<bool>(eReference->getEOpposite());
        return false;
    }

    template <typename... I>
    bool DynamicEObjectBase<I...>::isContainer(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const
    {
        if (auto eReference = std::dynamic_pointer_cast<EReference>(eStructuralFeature))
        {
            if (auto opposite = eReference->getEOpposite())
                return static_cast<bool>(opposite->isContainment());
        }
        return false;
    }

    template <typename... I>
    bool DynamicEObjectBase<I...>::isContains(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const
    {
        if (auto eReference = std::dynamic_pointer_cast<EReference>(eStructuralFeature))
            return eReference->isContainment();
        return false;
    }

    template <typename... I>
    bool DynamicEObjectBase<I...>::isBackReference(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const
    {
        if (auto eReference = std::dynamic_pointer_cast<EReference>(eStructuralFeature))
            return eReference->isContainer();
        return false;
    }

    template <typename... I>
    bool DynamicEObjectBase<I...>::isProxy(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const
    {
        if (isContainer(eStructuralFeature) || isContains(eStructuralFeature))
            return false;
        if (auto eReference = std::dynamic_pointer_cast<EReference>(eStructuralFeature))
            return eReference->isResolveProxies();
        return false;
    }

    template <typename... I>
    void DynamicEObjectBase<I...>::resizeProperties()
    {
        properties_.resize(eClass()->getFeatureCount() - eStaticFeatureCount());
    }

    template <typename... I>
    std::shared_ptr<EList<std::shared_ptr<EObject>>> DynamicEObjectBase<I...>::createList(const std::shared_ptr<EStructuralFeature>& eStructuralFeature) const
    {
        if (auto eAttribute = std::dynamic_pointer_cast<EAttribute>(eStructuralFeature))
        {
            if (eAttribute->isUnique())
                return std::make_shared< ArrayEList<std::shared_ptr<EObject>, true> >();
            else
                return std::make_shared< ArrayEList<std::shared_ptr<EObject>, false> >();
        }
        else if (auto eReference = std::dynamic_pointer_cast<EReference>(eStructuralFeature))
        {
            if (eReference->isContainment())
            {
                // containment

                if (auto eReverseFeature = eReference->getEOpposite())
                {
                    //  opposite

                    if (eReference->eIsProxy())
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, true, true>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, true, false>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());

                    }
                    else
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, false, true>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, true, false, false>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());
                    }
                }
                else
                {
                    // no opposite

                    if (eReference->eIsProxy())
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, true, true>>(getThisPtr(), eReference->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, true, false>>(getThisPtr(), eReference->getFeatureID());
                    }
                    else
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, false, true>>(getThisPtr(), eReference->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, true, true, false, false, false>>(getThisPtr(), eReference->getFeatureID());
                    }
                }
            }
            else
            {
                // no containment

                if (auto eReverseFeature = eReference->getEOpposite())
                {
                    //  opposite

                    if (eReference->eIsProxy())
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, true, true>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, true, false>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());

                    }
                    else
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, false, true>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, true, true, false, false>>(getThisPtr(), eReference->getFeatureID(), eReverseFeature->getFeatureID());
                    }
                }
                else
                {
                    // no opposite

                    if (eReference->eIsProxy())
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, true, true>>(getThisPtr(), eReference->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, true, false>>(getThisPtr(), eReference->getFeatureID());
                    }
                    else
                    {
                        if (eReference->isUnsettable())
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, false, true>>(getThisPtr(), eReference->getFeatureID());
                        else
                            return std::make_shared<EObjectEList<std::shared_ptr<EObject>, false, false, false, false, false>>(getThisPtr(), eReference->getFeatureID());
                    }
                }
            }
        }
        return std::shared_ptr< EList<std::shared_ptr<EObject>> >();
    }

}