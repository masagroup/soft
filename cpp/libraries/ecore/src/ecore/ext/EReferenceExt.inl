// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EREFERENCEEXT_HPP
#error This file may only be included from EReferenceExt.hpp
#endif

#include "ecore/EClass.hpp"

namespace ecore::ext {

    template <typename... I>
    EReferenceBaseExt<I...>::EReferenceBaseExt()
    {
    }

    template <typename... I>
    EReferenceBaseExt<I...>::~EReferenceBaseExt()
    {
    }

    template <typename... I>
    bool EReferenceBaseExt<I...>::isContainer() const
    {
        std::shared_ptr<EReference> theOpposite = getEOpposite();
        return theOpposite && theOpposite->isContainment();
    }

    template <typename... I>
    void ecore::ext::EReferenceBaseExt<I...>::setEType(const std::shared_ptr<ecore::EClassifier>& newEType)
    {
        eReferenceType_.reset();
        EReferenceImpl::setEType(newEType);
    }

    template <typename... I>
    std::shared_ptr<EClass> EReferenceBaseExt<I...>::getEReferenceType() const
    {
        if (!eReferenceType_) {
            auto eType = getEType();
            auto eReferenceType = std::dynamic_pointer_cast<EClass>(eType);
            if (eReferenceType)
                eReferenceType_ = eReferenceType;
        }
        return eReferenceType_;
    }

    template <typename... I>
    std::shared_ptr<EClass> EReferenceBaseExt<I...>::basicGetEReferenceType() const
    {
        if (!eReferenceType_) {
            auto eType = basicGetEType();
            auto eReferenceType = std::dynamic_pointer_cast<EClass>(eType);
            if (eReferenceType)
                eReferenceType_ = eReferenceType;
        }
        return eReferenceType_;
    }

}
