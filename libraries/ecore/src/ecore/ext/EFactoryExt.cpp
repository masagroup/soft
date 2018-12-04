#include "ecore/ext/EFactoryExt.hpp"
#include "ecore/impl/DynamicEObject.hpp"
#include "ecore/EClass.hpp"

#include <stdexcept>

using namespace ecore;
using namespace ecore::ext;
using namespace ecore::impl;

EFactoryExt::EFactoryExt()
{
}

EFactoryExt::~EFactoryExt()
{
}

std::shared_ptr<ecore::EObject> EFactoryExt::create( const std::shared_ptr<ecore::EClass>& eClass ) const
{
    if( getEPackage() != eClass->getEPackage() || eClass->isAbstract() )
        throw std::invalid_argument( "The class '" + eClass->getName() + "' is not a valid classifier" );
    return std::make_shared<DynamicEObject>( eClass );
}
