// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EATTRIBUTEBASEEXT_HPP
#error This file may only be included from EAttributeBaseExt.hpp
#endif

#include "ecore/EDataType.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/ext/EClassExt.hpp"
#include "ecore/ext/EClassInternal.hpp"

namespace ecore::ext
{
    template <typename... I>
    EAttributeBaseExt<I...>::EAttributeBaseExt()
        : attributeType_([&]() { return initAttributeType(); })
    {
    }

    template <typename... I>
    EAttributeBaseExt<I...>::~EAttributeBaseExt()
    {
    }

    template <typename... I>
    std::shared_ptr<ecore::EDataType> EAttributeBaseExt<I...>::getEAttributeType() const
    {
        return attributeType_;
    }

    template <typename... I>
    std::shared_ptr<ecore::EDataType> EAttributeBaseExt<I...>::basicGetEAttributeType() const
    {
        return attributeType_;
    }

    template <typename... I>
    std::shared_ptr<EDataType> EAttributeBaseExt<I...>::initAttributeType()
    {
        auto eType = getEType();
        auto eDataType = std::static_pointer_cast<EDataType>(eType);
        return eDataType;
    }

    template <typename... I>
    void EAttributeBaseExt<I...>::setID(bool newID)
    {
        EAttributeBase<I...>::setID(newID);
        auto eClass = getEContainingClass();
        if( eClass )
            static_cast<EClassInternal&>( eClass->getInternal() ).setModified( EcorePackage::ECLASS__EATTRIBUTES );
    }

}