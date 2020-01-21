// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EPACKAGEBASEEXT_HPP
#error This file may only be included from EPackageExt.hpp
#endif

#include "ecore/ext/EFactoryExt.hpp"
#include "ecore/EAdapter.hpp"
#include "ecore/EClassifier.hpp"
#include "ecore/ENotification.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/EList.hpp"
#include "ecore/impl/AbstractAdapter.hpp"

namespace ecore::ext {

    template <typename... I>
    class EPackageBaseExt<I...>::Adapter : public AbstractAdapter
    {
    public:
        Adapter(EPackageBaseExt& ePackage)
            : ePackage_(ePackage)
        {
        }

        virtual void notifyChanged(const std::shared_ptr<ENotification>& notification)
        {
            int eventType = notification->getEventType();
            if (eventType != ENotification::REMOVING_ADAPTER)
            {
                int featureID = notification->getFeatureID();
                if (featureID == EcorePackage::EPACKAGE__ECLASSIFIERS)
                {
                    ePackage_.nameToClassifierMap_.reset();
                }
            }
        }

    private:
        EPackageBaseExt& ePackage_;
    };

    template <typename... I>
    EPackageBaseExt<I...>::EPackageBaseExt()
        : adapter_(std::make_unique<Adapter>(*this))
    {
    }

    template <typename... I>
    void EPackageBaseExt<I...>::setThisPtr(const std::shared_ptr<EPackageBaseExt>& thisPtr)
    {
        EPackageImpl::setThisPtr(thisPtr);
        eAdapters().add(adapter_.get());
    }

    template <typename... I>
    EPackageBaseExt<I...>::~EPackageBaseExt()
    {
        eAdapters().remove(adapter_.get());
    }

    template <typename... I>
    std::shared_ptr<EClassifier> EPackageBaseExt<I...>::getEClassifier(const std::string& name)
    {
        if (!nameToClassifierMap_)
        {
            nameToClassifierMap_ = std::make_unique< std::unordered_map< std::string, std::shared_ptr<EClassifier>>>();
            for (const auto& eClassifier : *getEClassifiers())
                nameToClassifierMap_->operator[](eClassifier->getName()) = eClassifier;
        }
        return nameToClassifierMap_->operator[](name);
    }

}