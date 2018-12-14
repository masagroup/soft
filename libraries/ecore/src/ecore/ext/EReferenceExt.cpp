#include "ecore/ext/EReferenceExt.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::ext;

EReferenceExt::EReferenceExt()
{
}

EReferenceExt::~EReferenceExt()
{
}

bool EReferenceExt::isContainer() const
{
    std::shared_ptr<EReference> theOpposite = getEOpposite();
    return theOpposite && theOpposite->isContainment();
}
