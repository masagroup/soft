#include "ecore/ext/EClassifierExt.hpp"
#include "ecore/EList.hpp"
#include "ecore/EPackage.hpp"


using namespace ecore;
using namespace ecore::ext;
using namespace ecore::impl;

EClassifierExt::EClassifierExt()
{
}

EClassifierExt::~EClassifierExt()
{
}

bool EClassifierExt::isInstance( const Any& object ) const
{
    return getInstanceClass() && getInstanceClass()->operator==( object.type() );
}

int EClassifierExt::initClassifierID()
{
    return getEPackage() ? static_cast<int>( getEPackage()->getEClassifiers()->indexOf( getThisPtr() ) ) : -1;
}
