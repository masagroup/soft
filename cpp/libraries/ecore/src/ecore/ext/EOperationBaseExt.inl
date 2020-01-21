// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EOPERATIONBASEEXT_HPP
#error This file may only be included from EOperationExt.hpp
#endif

#include "ecore/EParameter.hpp"
#include "ecore/EClass.hpp"

namespace ecore::ext
{
    template <typename... I>
    EOperationBaseExt<I...>::EOperationBaseExt()
    {
    }

    template <typename... I>
    EOperationBaseExt<I...>::~EOperationBaseExt()
    {
    }

    template <typename... I>
    bool EOperationBaseExt<I...>::isOverrideOf(const std::shared_ptr<ecore::EOperation>& someOperation)
    {
        if (someOperation->getEContainingClass()->isSuperTypeOf(getEContainingClass()) && someOperation->getName() == getName())
        {
            auto parameters = getEParameters();
            auto otherParameters = someOperation->getEParameters();
            if (parameters->size() == otherParameters->size())
            {
                for (int i = 0; i < parameters->size(); ++i)
                {
                    auto parameter = parameters->get(i);
                    auto otherParameter = otherParameters->get(i);
                    if (parameter->getEType() != otherParameter->getEType())
                        return false;
                }
                return true;
            }
        }
        return false;
    }
}