#include "ecore/ext/EcoreFactoryExt.hpp"
#include "ecore/ext/EClassExt.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::ext;

EcoreFactoryExt::EcoreFactoryExt()
{
}

EcoreFactoryExt::~EcoreFactoryExt()
{
}

std::shared_ptr<EClass> EcoreFactoryExt::createEClass() const
{
    return std::make_shared<EClassExt>();
}

EcoreFactoryExt * EcoreFactoryExt::create()
{
    return new EcoreFactoryExt();
}
