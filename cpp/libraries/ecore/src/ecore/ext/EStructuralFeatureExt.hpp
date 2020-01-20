// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ESTRUCTURALFEATUREEXT_HPP
#define ECORE_EXT_ESTRUCTURALFEATUREEXT_HPP

#include "ecore/impl/EStructuralFeatureImpl.hpp"

namespace ecore
{
    class EFactory;
}

namespace ecore::ext
{
    template <typename... I>
    class EStructuralFeatureBaseExt : public ecore::impl::EStructuralFeatureBase<I...>
    {
    private:
        EStructuralFeatureBaseExt& operator=(EStructuralFeatureBaseExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EStructuralFeatureBaseExt();

    public:
        virtual ~EStructuralFeatureBaseExt();

        virtual const Any& getDefaultValue() const;

        virtual void setDefaultValue( const Any& newDefaultValue );

        virtual void setDefaultValueLiteral( const std::string& newDefaultValueLiteral );

    private:
        mutable Any defaultValue_;
        mutable std::weak_ptr<EFactory> defaultValueFactory_;
    };

    typedef EStructuralFeatureBaseExt<EStructuralFeature> EStructuralFeatureExt;

}

#include "ecore/ext/EStructuralFeatureExt.inl"

#endif /* ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP */