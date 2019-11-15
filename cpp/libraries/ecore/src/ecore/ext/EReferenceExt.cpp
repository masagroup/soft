#include "ecore/ext/EReferenceExt.hpp"
#include "ecore/EClass.hpp"

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

void ecore::ext::EReferenceExt::setEType(const std::shared_ptr<ecore::EClassifier>& newEType)
{
    eReferenceType_.reset();
    EReferenceImpl::setEType(newEType);
}

std::shared_ptr<EClass> EReferenceExt::getEReferenceType() const
{
    if (!eReferenceType_) {
        auto eType = getEType();
        auto eReferenceType = std::dynamic_pointer_cast<EClass>(eType);
        if (eReferenceType)
            eReferenceType_ = eReferenceType;
    }
    return eReferenceType_;
}

std::shared_ptr<EClass> EReferenceExt::basicGetEReferenceType() const
{
    if (!eReferenceType_) {
        auto eType = basicGetEType();
        auto eReferenceType = std::dynamic_pointer_cast<EClass>(eType);
        if (eReferenceType)
            eReferenceType_ = eReferenceType;
    }
    return eReferenceType_;
}
