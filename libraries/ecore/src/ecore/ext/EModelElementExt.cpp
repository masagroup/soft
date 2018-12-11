#include "ecore/ext/EModelElementExt.hpp"
#include "ecore/EAnnotation.hpp"
#include "ecore/EList.hpp"

#include <algorithm>

using namespace ecore;
using namespace ecore::ext;
using namespace ecore::impl;

EModelElementExt::EModelElementExt()
{
}

EModelElementExt::~EModelElementExt()
{
}

std::shared_ptr<ecore::EAnnotation> EModelElementExt::getEAnnotation( const std::string & source )
{
    if( eAnnotations_ )
    {
        auto it = std::find_if( eAnnotations_->begin(), eAnnotations_->end(), [ = ]( const auto& a )
        {
            return source == a->getSource();
        } );
        if( it != eAnnotations_->end() )
            return *it;
    }
    return std::shared_ptr<EAnnotation>();
}
