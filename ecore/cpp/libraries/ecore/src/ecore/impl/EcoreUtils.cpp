#include "ecore/impl/EcoreUtils.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EFactory.hpp"

using namespace ecore;
using namespace ecore::impl;

std::string EcoreUtils::getID( const std::shared_ptr<EObject>& eObject )
{
    std::shared_ptr<EClass> eClass = eObject->eClass();
    std::shared_ptr<EAttribute> eIDAttribute = eClass->getEIDAttribute();
    return !eIDAttribute || !eObject->eIsSet( eIDAttribute )
               ? std::string()
               : convertToString( eIDAttribute->getEAttributeType(), eObject->eGet( eIDAttribute ) );

    return std::string();
}

void EcoreUtils::setID( const std::shared_ptr<EObject>& eObject, const std::string& id )
{
    std::shared_ptr<EClass> eClass = eObject->eClass();
    std::shared_ptr<EAttribute> eIDAttribute = eClass->getEIDAttribute();
    if( !eIDAttribute)
        throw std::runtime_error( "The object doesn't have an ID feature." );
    else if( id.empty() )
        eObject->eUnset( eIDAttribute );
    else
        eObject->eSet( eIDAttribute, createFromString( eIDAttribute->getEAttributeType(), id ) );
}

std::string EcoreUtils::convertToString( const std::shared_ptr<EDataType>& eDataType, const Any& value )
{
    auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
    return eFactory->convertToString(eDataType,value);
}

Any EcoreUtils::createFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literal )
{
    auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
    return eFactory->createFromString( eDataType, literal );
}
