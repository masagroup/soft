// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP
#define ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP

#include "ecore/impl/EStructuralFeatureImpl.hpp"

namespace ecore
{
    class EFactory;
}

namespace ecore::ext
{
    class EStructuralFeatureExt : public virtual impl::EStructuralFeatureImpl
    {
    private:
        EStructuralFeatureExt& operator=( EStructuralFeatureExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EStructuralFeatureExt();

    public:
        virtual ~EStructuralFeatureExt();

        virtual const Any& getDefaultValue() const;

        virtual void setDefaultValue( const Any& newDefaultValue );

        virtual void setDefaultValueLiteral( const std::string& newDefaultValueLiteral );

    private:
        mutable Any defaultValue_;
        mutable std::weak_ptr<EFactory> defaultValueFactory_;
    };

}

#endif /* ECORE_ESTRUCTURALFEATURE_ESTRUCTURALFEATUREEXT_HPP */