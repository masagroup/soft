#include "ecore/ext/EAttributeExt.hpp"
#include "ecore/ext/EClassExt.hpp"
#include "ecore/EcorePackage.hpp"

using namespace ecore;
using namespace ecore::ext;
using namespace ecore::impl;

EAttributeExt::EAttributeExt()
{
}

void EAttributeExt::setID( bool newID )
{
    EAttributeImpl::setID( newID );
    std::shared_ptr<EClassExt> eClass = std::dynamic_pointer_cast<EClassExt>( getEContainingClass() );
    if( eClass )
        eClass->setModified( EcorePackage::ECLASS__EATTRIBUTES );
}

EAttributeExt::~EAttributeExt()
{
}
