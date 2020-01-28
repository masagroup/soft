// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ESTRUCTURALFEATUREBASEEXT_HPP
#error This file may only be included from EStructuralFeatureBaseExt.hpp
#endif

#include "ecore/Constants.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EPackage.hpp"

namespace ecore::ext
{

    template <typename... I>
    EStructuralFeatureBaseExt<I...>::EStructuralFeatureBaseExt()
    {
    }

    template <typename... I>
    EStructuralFeatureBaseExt<I...>::~EStructuralFeatureBaseExt()
    {
    }

    template <typename... I>
    const Any& EStructuralFeatureBaseExt<I...>::getDefaultValue() const
    {
        auto eType = getEType();
        auto literal = getDefaultValueLiteral();
        if( eType && literal.empty() )
            return isMany() ? NO_VALUE : eType->getDefaultValue();
        else if( auto eDataType = std::dynamic_pointer_cast<EDataType>( eType ) )
        {
            auto ePackage = eDataType->getEPackage();
            if( ePackage )
            {
                auto eFactory = ePackage->getEFactoryInstance();
                if( eFactory != defaultValueFactory_.lock() )
                {
                    defaultValueFactory_ = eFactory;

                    if( eDataType->isSerializable() )
                        defaultValue_ = eFactory->createFromString( eDataType, literal );
                }
            }
            return defaultValue_;
        }
        return NO_VALUE;
    }

    template <typename... I>
    void EStructuralFeatureBaseExt<I...>::setDefaultValue( const Any& newDefaultValue )
    {
        auto eType = getEType();
        if( auto eDataType = std::dynamic_pointer_cast<EDataType>( eType ) )
        {
            auto eFactory = eType->getEPackage()->getEFactoryInstance();
            auto literal = eFactory->convertToString( eDataType, newDefaultValue );
            setDefaultValueLiteral( literal );
        }
        else
            throw "Cannot serialize value to object without an EDataType eType";
    }

    template <typename... I>
    void EStructuralFeatureBaseExt<I...>::setDefaultValueLiteral( const std::string& newDefaultValueLiteral )
    {
        EStructuralFeatureBase<I...>::setDefaultValueLiteral( newDefaultValueLiteral );
        defaultValueFactory_.reset();
    }

    template <typename... I>
    template <typename U>
    class EStructuralFeatureBaseExt<I...>::EObjectInternalAdapter : public AbstractEObject<I...>::EObjectInternalAdapter<typename U>
    {
    public:
        EObjectInternalAdapter( EStructuralFeatureBaseExt<I...>& obj )
            : AbstractEObject<I...>::EObjectInternalAdapter<typename U>( obj )
        {
        }

        virtual void setFeatureID( int newFeatureID )
        {
            getObject().setFeatureID( newFeatureID );
        }

        inline EStructuralFeatureBaseExt<I...>& getObject()
        {
            return static_cast<EStructuralFeatureBaseExt<I...>&>( AbstractEObject<I...>::EObjectInternalAdapter<typename U>::getObject() );
        }
    };

    template <typename... I>
    std::unique_ptr<impl::EObjectInternal> EStructuralFeatureBaseExt<I...>::createInternal()
    {
        return std::move( std::make_unique<EObjectInternalAdapter<EStructuralFeatureInternal>>( *this ) );
    }

} // namespace ecore::ext
