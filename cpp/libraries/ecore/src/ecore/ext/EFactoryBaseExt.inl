// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EFACTORYBASEEXT_HPP
#error This file may only be included from EFactoryBaseExt.hpp
#endif


#include "ecore/impl/DynamicEObjectImpl.hpp"
#include "ecore/EClass.hpp"

#include <stdexcept>

namespace ecore::ext {

    template <typename... I>
    EFactoryBaseExt<I...>::EFactoryBaseExt()
    {
    }

    template <typename... I>
    EFactoryBaseExt<I...>::~EFactoryBaseExt()
    {
    }

    template <typename... I>
    std::shared_ptr<ecore::EObject> EFactoryBaseExt<I...>::create(const std::shared_ptr<ecore::EClass>& eClass) const
    {
        if (getEPackage() != eClass->getEPackage() || eClass->isAbstract())
            throw std::invalid_argument("The class '" + eClass->getName() + "' is not a valid classifier");

        auto eObject = std::make_shared<DynamicEObjectImpl>(eClass);
        eObject->setThisPtr(eObject);
        return eObject;
    }

}