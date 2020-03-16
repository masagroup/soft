// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EFEATUREMAP_HPP_
#define ECORE_EFEATUREMAP_HPP_

#include "ecore/EList.hpp"

namespace ecore
{
    class EStructuralFeature;
}

namespace ecore
{
    class EFeatureMapEntry
    {
    public:
        virtual ~EFeatureMapEntry() = default;

        virtual std::shared_ptr<EStructuralFeature> getFeature() const = 0;

        virtual Any getValue() const = 0;
    };


    class EFeatureMap : public EList<std::shared_ptr<EFeatureMapEntry>>
    {
    public:
        virtual ~EFeatureMap() = default;
    };

} // namespace ecore

#endif /* ECORE_EFEATUREMAP_HPP_ */
