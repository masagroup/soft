#include "ecore/ext/EAttributeExt.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/ext/EClassExt.hpp"

using namespace ecore;
using namespace ecore::ext;
using namespace ecore::impl;

EAttributeExt::EAttributeExt()
    : attributeType_( [&]() { return initAttributeType(); } )
{
}

EAttributeExt::~EAttributeExt()
{
}

std::shared_ptr<ecore::EDataType> EAttributeExt::getEAttributeType() const
{
    return attributeType_;
}

std::shared_ptr<ecore::EDataType> EAttributeExt::basicGetEAttributeType() const
{
    return attributeType_;
}

std::shared_ptr<EDataType> EAttributeExt::initAttributeType()
{
    auto eType = getEType();
    auto eDataType = std::dynamic_pointer_cast<EDataType>( eType );
    return eDataType;
}

void EAttributeExt::setID( bool newID )
{
    EAttributeImpl::setID( newID );
    std::shared_ptr<EClassExt> eClass = std::dynamic_pointer_cast<EClassExt>( getEContainingClass() );
    if( eClass )
        eClass->setModified( EcorePackage::ECLASS__EATTRIBUTES );
}
