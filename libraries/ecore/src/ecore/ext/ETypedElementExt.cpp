#include "ecore/ext/ETypedElementExt.hpp"
#include "ecore/Constants.hpp"

using namespace ecore;
using namespace ecore::ext;

ETypedElementExt::ETypedElementExt()
{
}

ETypedElementExt::~ETypedElementExt()
{
}

bool ETypedElementExt::isMany() const
{
    int upper = getUpperBound();
    return upper > 1 || upper == UNBOUNDED_MULTIPLICITY;
}


bool ETypedElementExt::isRequired() const
{
    int lower = getLowerBound();
    return lower >= 1;
}