#include "ecore/ext/EOperationExt.hpp"
#include "ecore/EParameter.hpp"
#include "ecore/EClass.hpp"

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::ext;

EOperationExt::EOperationExt()
{
}

EOperationExt::~EOperationExt()
{
}

bool EOperationExt::isOverrideOf( const std::shared_ptr<ecore::EOperation>& someOperation )
{
    if (someOperation->getEContainingClass()->isSuperTypeOf( getEContainingClass() ) && someOperation->getName() == getName())
    {
        auto parameters = getEParameters();
        auto otherParameters = someOperation->getEParameters();
        if (parameters->size() == otherParameters->size())
        {
            for (int i = 0; i < parameters->size(); ++i)
            {
                auto parameter = parameters->get( i );
                auto otherParameter = otherParameters->get( i );
                if (parameter->getEType() != otherParameter->getEType())
                    return false;
            }
            return true;
        }
    }
    return false;
}
