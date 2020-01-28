// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ESTRUCTURALFEATUREBASEEXT_HPP
#define ECORE_EXT_ESTRUCTURALFEATUREBASEEXT_HPP

#include "ecore/impl/EStructuralFeatureBase.hpp"
#include "ecore/ext/EStructuralFeatureInternal.hpp"

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
        template<typename... I> friend class EcoreFactoryBase;
        EStructuralFeatureBaseExt();

    public:
        virtual ~EStructuralFeatureBaseExt();

        virtual const Any& getDefaultValue() const;

        virtual void setDefaultValue( const Any& newDefaultValue );

        virtual void setDefaultValueLiteral( const std::string& newDefaultValueLiteral );

    protected:
        template <typename U>
        class EObjectInternalAdapter;

        virtual std::unique_ptr<impl::EObjectInternal> createInternal();

    private:
        mutable Any defaultValue_;
        mutable std::weak_ptr<EFactory> defaultValueFactory_;
    };
}

#include "ecore/ext/EStructuralFeatureBaseExt.inl"

#endif /* ECORE_EXT_ESTRUCTURALFEATUREBASEEXT_HPP */