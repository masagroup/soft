#include "ecore/ext/EStructuralFeatureExt.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/Constants.hpp"

using namespace ecore;
using namespace ecore::ext;
using namespace ecore::impl;

EStructuralFeatureExt::EStructuralFeatureExt()
{
}

EStructuralFeatureExt::~EStructuralFeatureExt()
{
}

const Any & EStructuralFeatureExt::getDefaultValue() const
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


void EStructuralFeatureExt::setDefaultValue( const Any& newDefaultValue )
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

void EStructuralFeatureExt::setDefaultValueLiteral( const std::string & newDefaultValueLiteral )
{
    EStructuralFeatureImpl::setDefaultValueLiteral( newDefaultValueLiteral );
    defaultValueFactory_.reset();
}


