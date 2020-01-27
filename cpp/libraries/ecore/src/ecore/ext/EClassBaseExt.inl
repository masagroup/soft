// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ECLASSBASEEXT_HPP
#error This file may only be included from EClassBaseExt.hpp
#endif


#include "ecore/EAdapter.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EOperation.hpp"
#include "ecore/EReference.hpp"
#include "ecore/impl/AbstractAdapter.hpp"
#include "ecore/impl/ImmutableEList.hpp"
#include "ecore/impl/EOperationImpl.hpp"
#include "ecore/impl/EStructuralFeatureImpl.hpp"

#include <algorithm>

namespace ecore::ext
{
    template <typename... I>
    class EClassBaseExt<I...>::ESuperAdapter : public ecore::impl::AbstractAdapter
    {
    public:
        ESuperAdapter(EClassBaseExt& eClassExt)
            : eClassExt_(eClassExt)
            , subClasses_()
        {
        }

        virtual ~ESuperAdapter()
        {

        }

        std::vector<std::weak_ptr<EClassBaseExt>>& getSubClasses()
        {
            return subClasses_;
        }

        virtual void notifyChanged(const std::shared_ptr<ENotification>& notification)
        {
            int eventType = notification->getEventType();
            auto eNotifier = std::static_pointer_cast<EClassBaseExt>(notification->getNotifier());
            if (eventType != ENotification::REMOVING_ADAPTER)
            {
                int featureID = notification->getFeatureID();
                if (featureID == EcorePackage::ECLASS__ESUPER_TYPES)
                {
                    switch (eventType)
                    {
                    case ENotification::SET:
                    case ENotification::RESOLVE:
                    {
                        Any oldValue = notification->getOldValue();
                        if (!oldValue.empty())
                        {
                            auto eClass = anyCast<std::shared_ptr<EClass>>(oldValue);
                            auto eClassImpl = std::static_pointer_cast<EClassBaseExt>(eClass);
                            auto& subClasses = eClassImpl->eSuperAdapter_->getSubClasses();
                            auto it = std::find_if(subClasses.begin(), subClasses.end(), [=](const auto& w)
                                {
                                    return w.lock() = eNotifier;
                                });
                            if (it != subClasses.end())
                                subClasses.erase(it);
                        }
                        Any newValue = notification->getNewValue();
                        if (!newValue.empty())
                        {
                            auto eClass = anyCast<std::shared_ptr<EClass>>(newValue);
                            auto eClassImpl = std::static_pointer_cast<EClassBaseExt>(eClass);
                            auto& subClasses = eClassImpl->eSuperAdapter_->getSubClasses();
                            subClasses.push_back(eNotifier);
                        }
                        break;
                    }
                    case ENotification::ADD:
                    {
                        Any newValue = notification->getNewValue();
                        if (!newValue.empty())
                        {
                            auto eClass = anyCast<std::shared_ptr<EClass>>(newValue);
                            auto eClassImpl = std::static_pointer_cast<EClassBaseExt>(eClass);
                            auto& subClasses = eClassImpl->eSuperAdapter_->getSubClasses();
                            subClasses.push_back(eNotifier);
                        }
                        break;
                    }
                    case ENotification::ADD_MANY:
                    {
                        Any newValue = notification->getNewValue();
                        if (!newValue.empty())
                        {
                            auto eCollection = anyCast<std::shared_ptr<EList<std::shared_ptr<EClass>>>>(newValue);
                            for (const auto& eClass : *eCollection)
                            {
                                auto eClassImpl = std::static_pointer_cast<EClassBaseExt>(eClass);
                                auto& subClasses = eClassImpl->eSuperAdapter_->getSubClasses();
                                subClasses.push_back(eNotifier);
                            }
                        }
                        break;
                    }
                    case ENotification::REMOVE:
                    {
                        Any oldValue = notification->getOldValue();
                        if (!oldValue.empty())
                        {
                            auto eClass = anyCast<std::shared_ptr<EClass>>(oldValue);
                            auto eClassImpl = std::static_pointer_cast<EClassBaseExt>(eClass);
                            auto& subClasses = eClassImpl->eSuperAdapter_->getSubClasses();
                            auto it = std::find_if(subClasses.begin(), subClasses.end(), [=](const auto& w)
                                {
                                    return w.lock() = eNotifier;
                                });
                            if (it != subClasses.end())
                                subClasses.erase(it);
                        }
                        break;
                    }
                    case ENotification::REMOVE_MANY:
                    {
                        Any oldValue = notification->getOldValue();
                        if (!oldValue.empty())
                        {
                            auto eCollection = anyCast<std::shared_ptr<EList<std::shared_ptr<EClass>>>>(oldValue);
                            for (const auto& eClass : *eCollection)
                            {
                                auto eClassImpl = std::static_pointer_cast<EClassBaseExt>(eClass);
                                auto& subClasses = eClassImpl->eSuperAdapter_->getSubClasses();
                                auto it = std::find_if(subClasses.begin(), subClasses.end(), [=](const auto& w)
                                    {
                                        return w.lock() = eNotifier;
                                    });
                                if (it != subClasses.end())
                                    subClasses.erase(it);
                            }
                        }
                        break;
                    }
                    }
                }
                eClassExt_.setModified(featureID);
            }
        }

    private:
        EClassBaseExt& eClassExt_;
        std::vector<std::weak_ptr<EClassBaseExt>> subClasses_;
    };

    template <typename... I>
    EClassBaseExt<I...>::EClassBaseExt()
        : EClassBase<I...>()
        , eSuperAdapter_(new ESuperAdapter(*this))
    {
    }

    template <typename... I>
    void EClassBaseExt<I...>::setThisPtr(const std::shared_ptr<EClassBaseExt>& thisPtr)
    {
        EClassBase<I...>::setThisPtr(thisPtr);
        eAdapters().add(eSuperAdapter_.get());
    }

    template <typename... I>
    EClassBaseExt<I...>::~EClassBaseExt()
    {
        eAdapters().remove(eSuperAdapter_.get());
    }

    template <typename... I>
    bool EClassBaseExt<I...>::isSuperTypeOf(const std::shared_ptr<ecore::EClass>& someClass)
    {
        return someClass == getThisPtr() || someClass->getEAllSuperTypes()->contains(getThisPtr());
    }

    template <typename... I>
    int EClassBaseExt<I...>::getFeatureCount()
    {
        return static_cast<int>(getEAllStructuralFeatures()->size());
    }

    template <typename... I>
    std::shared_ptr<ecore::EStructuralFeature> EClassBaseExt<I...>::getEStructuralFeature(int featureID)
    {
        auto features = getEAllStructuralFeatures();
        return featureID >= 0 && featureID < features->size() ? features->get(featureID) : std::shared_ptr<EStructuralFeature>();
    }

    template <typename... I>
    std::shared_ptr<ecore::EStructuralFeature> EClassBaseExt<I...>::getEStructuralFeature(const std::string& featureName)
    {
        initNameToFeatureMap();
        auto it = nameToFeatureMap_->find(featureName);
        return it != nameToFeatureMap_->end() ? it->second : std::shared_ptr<EStructuralFeature>();
    }

    template <typename... I>
    int EClassBaseExt<I...>::getFeatureID(const std::shared_ptr<ecore::EStructuralFeature>& feature)
    {
        auto features = getEAllStructuralFeatures();
        int featureID = feature->getFeatureID();
        if (featureID != -1)
        {
            for (; featureID < features->size(); ++featureID)
            {
                if (features->get(featureID) == feature)
                    return featureID;
            }
        }
        return -1;
    }

    template <typename... I>
    int EClassBaseExt<I...>::getOperationCount()
    {
        return static_cast<int>(getEAllOperations()->size());
    }

    template <typename... I>
    std::shared_ptr<ecore::EOperation> EClassBaseExt<I...>::getEOperation(int operationID)
    {
        auto operations = getEAllOperations();
        return operationID >= 0 && operationID < operations->size() ? operations->get(operationID) : std::shared_ptr<EOperation>();
    }

    template <typename... I>
    int EClassBaseExt<I...>::getOperationID(const std::shared_ptr<ecore::EOperation>& operation)
    {
        auto operations = getEAllOperations();
        int operationID = operation->getOperationID();
        if (operationID != -1)
        {
            for (; operationID < operations->size(); ++operationID)
            {
                if (operations->get(operationID) == operation)
                    return operationID;
            }
        }
        return -1;
    }

    template <typename... I>
    std::shared_ptr<ecore::EOperation> EClassBaseExt<I...>::getOverride(const std::shared_ptr<ecore::EOperation>& operation)
    {
        initOperationToOverrideMap();
        auto it = operationToOverrideMap_->find(operation);
        return it != operationToOverrideMap_->end() ? it->second : std::shared_ptr<EOperation>();
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEAllAttributes()
    {
        if (eAllAttributes_)
            return;

        std::vector<std::shared_ptr<EAttribute>> allAttributes, attributes;
        std::shared_ptr<EAttribute> eIDAttribute;
        for (const auto& eClass : *getESuperTypes())
        {
            auto superAttributes = eClass->getEAllAttributes();
            for (const auto& attribute : *superAttributes)
            {
                allAttributes.push_back(attribute);
                if (attribute->isID() && !eIDAttribute)
                    eIDAttribute = attribute;
            }
        }
        auto features = getEStructuralFeatures();
        for (const auto& feature : *features)
        {
            if (auto attribute = std::dynamic_pointer_cast<EAttribute>(feature))
            {
                attributes.push_back(attribute);
                allAttributes.push_back(attribute);
                if (attribute->isID() && !eIDAttribute)
                    eIDAttribute = attribute;
            }
        }

        eIDAttribute_ = eIDAttribute;
        eAttributes_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EAttribute>>>(std::move(attributes));
        eAllAttributes_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EAttribute>>>(std::move(allAttributes));
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEAllReferences()
    {
        if (eAllReferences_)
            return;

        std::vector<std::shared_ptr<EReference>> allReferences, references;

        for (const auto& eClass : *getESuperTypes())
        {
            auto superReferences = eClass->getEAllReferences();
            allReferences.insert(std::end(allReferences), superReferences->begin(), superReferences->end());
        }
        auto features = getEStructuralFeatures();
        for (const auto& feature : *features)
        {
            if (auto reference = std::dynamic_pointer_cast<EReference>(feature))
            {
                references.push_back(reference);
                allReferences.push_back(reference);
            }
        }
        eReferences_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EReference>>>(std::move(references));
        eAllReferences_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EReference>>>(std::move(allReferences));
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEReferences()
    {
        initEAllReferences();
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEAttributes()
    {
        initEAllAttributes();
    }

    template <typename... I>
    void ecore::ext::EClassBaseExt<I...>::initEContainments()
    {
        initFeaturesSubSet();
    }

    template <typename... I>
    void ecore::ext::EClassBaseExt<I...>::initECrossReferences()
    {
        initFeaturesSubSet();
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEAllContainments()
    {
        if (eAllContainments_)
            return;

        std::vector< std::shared_ptr< EReference > > allContainments;
        for (const auto& reference : *getEAllReferences())
        {
            if (reference->isContainment())
                allContainments.push_back(reference);
        }
        eAllContainments_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EReference>>>(std::move(allContainments));
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEAllOperations()
    {
        if (eAllOperations_)
            return;

        operationToOverrideMap_.reset();

        std::vector< std::shared_ptr< EOperation > > allOperations;
        for (const auto& eClass : *getESuperTypes())
        {
            auto operations = eClass->getEAllOperations();
            allOperations.insert(std::end(allOperations), operations->begin(), operations->end());
        }
        int operationID = static_cast<int>(allOperations.size());
        for (const auto& operation : *getEOperations())
        {
            auto operationImpl = std::dynamic_pointer_cast<EOperationExt>(operation);
            _ASSERT(operationImpl);
            operationImpl->setOperationID(operationID++);
            allOperations.push_back(operationImpl);
        }
        eAllOperations_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EOperation>>>(std::move(allOperations));
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEAllStructuralFeatures()
    {
        if (eAllStructuralFeatures_)
            return;

        eCrossReferences_.reset();
        eContainments_.reset();
        nameToFeatureMap_.reset();

        std::vector< std::shared_ptr< EStructuralFeature > > allFeatures;
        for (const auto& eClass : *getESuperTypes())
        {
            auto features = eClass->getEAllStructuralFeatures();
            allFeatures.insert(std::end(allFeatures), features->begin(), features->end());
        }
        int featureID = static_cast<int>(allFeatures.size());
        for (const auto& feature : *getEStructuralFeatures())
        {
            auto featureImpl = std::dynamic_pointer_cast<EStructuralFeatureExt>(feature);
            _ASSERT(featureImpl);
            featureImpl->setFeatureID(featureID++);
            allFeatures.push_back(featureImpl);
        }
        eAllStructuralFeatures_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EStructuralFeature>>>(std::move(allFeatures));
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEAllSuperTypes()
    {
        if (eAllSuperTypes_)
            return;

        std::vector< std::shared_ptr< EClass> > allSuperTypes;
        for (const auto& eClass : *getESuperTypes())
        {
            auto superTypes = eClass->getEAllSuperTypes();
            allSuperTypes.insert(std::end(allSuperTypes), superTypes->begin(), superTypes->end());
            allSuperTypes.push_back(eClass);
        }
        eAllSuperTypes_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EClass>>>(std::move(allSuperTypes));
    }

    template <typename... I>
    void EClassBaseExt<I...>::initEIDAttribute()
    {
        initEAllAttributes();
    }

    template <typename... I>
    void EClassBaseExt<I...>::setModified(int featureID)
    {
        switch (featureID)
        {
        case EcorePackage::ECLASS__ESTRUCTURAL_FEATURES:
        {
            eAllAttributes_.reset();
            eAllStructuralFeatures_.reset();
            eAllReferences_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__EATTRIBUTES:
        {
            eAllAttributes_.reset();
            eAllStructuralFeatures_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__EREFERENCES:
        {
            eAllReferences_.reset();
            eAllStructuralFeatures_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__EOPERATIONS:
        {
            eAllOperations_.reset();
            eAllContainments_.reset();
            break;
        }
        case EcorePackage::ECLASS__ESUPER_TYPES:
        {
            eAllSuperTypes_.reset();
            eAllAttributes_.reset();
            eAllOperations_.reset();
            eAllStructuralFeatures_.reset();
            eAllReferences_.reset();
            eAllContainments_.reset();
            break;
        }
        }
        const auto& subClasses = eSuperAdapter_->getSubClasses();
        for (const auto& weakSubClass : subClasses)
        {
            if (auto subClass = weakSubClass.lock())
                subClass->setModified(featureID);
        }
    }

    template <typename... I>
    void EClassBaseExt<I...>::initFeaturesSubSet()
    {
        initEAllStructuralFeatures();

        if (eContainments_)
            return;

        std::vector< std::shared_ptr< EReference > > containments;
        std::vector< std::shared_ptr< EReference > > crossReferences;
        auto eFeatures = getEStructuralFeatures();
        for (const auto& feature : *eFeatures)
        {
            if (auto reference = std::dynamic_pointer_cast<EReference>(feature))
            {
                if (reference->isContainment())
                {
                    if (!reference->isDerived())
                        containments.push_back(reference);
                }
                else if (!reference->isContainer())
                {
                    if (!reference->isDerived())
                        crossReferences.push_back(reference);
                }
            }
        }
        eContainments_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EReference>>>(std::move(containments));
        eCrossReferences_ = std::make_shared< impl::ImmutableEList<std::shared_ptr<EReference>>>(std::move(crossReferences));
    }

    template <typename... I>
    void EClassBaseExt<I...>::initNameToFeatureMap()
    {
        initEAllStructuralFeatures();

        if (nameToFeatureMap_)
            return;

        nameToFeatureMap_ = std::make_unique< std::unordered_map< std::string, std::shared_ptr<EStructuralFeature>>>();
        for (const auto& feature : *getEAllStructuralFeatures())
            nameToFeatureMap_->operator[](feature->getName()) = feature;
    }

    template <typename... I>
    void EClassBaseExt<I...>::initOperationToOverrideMap()
    {
        initEAllOperations();

        if (operationToOverrideMap_)
            return;

        operationToOverrideMap_ = std::make_unique< std::unordered_map< std::shared_ptr<EOperation>, std::shared_ptr<EOperation>>>();
        auto& allOperations = *getEAllOperations();
        auto size = static_cast<int>(allOperations.size());
        for (int i = 0; i < size; ++i)
        {
            for (int j = size - 1; j > i; --j)
            {
                const auto& oi = allOperations.get(i);
                const auto& oj = allOperations.get(j);
                if (oj->isOverrideOf(oi))
                    operationToOverrideMap_->operator[](oi) = oj;
            }
        }
    }

}