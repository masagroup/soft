#include "ecore/ext/EModelElementExt.hpp"
#include "ecore/EAnnotation.hpp"
#include "ecore/ENamedElement.hpp"
#include "ecore/EList.hpp"

#include <algorithm>
#include <sstream>

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

std::shared_ptr<EObject> EModelElementExt::eObjectForFragmentSegment(const std::string& uriSegment) const
{
    return std::shared_ptr<EObject>();
}

std::string  EModelElementExt::eURIFragmentSegment(const std::shared_ptr<EStructuralFeature>& feature, const std::shared_ptr<EObject>& eObject) const
{
    auto eNamedElement = std::dynamic_pointer_cast<ENamedElement>(eObject);
    if (eNamedElement) {
        auto name = eNamedElement->getName();
        auto contents = eContents();
        int count = 0;
        for ( auto otherEObject : *contents )
        {
            if (otherEObject == eObject)
                break;

            auto otherENamedElement = std::dynamic_pointer_cast<ENamedElement>(otherEObject);
            if (otherENamedElement )
            {
                auto otherName = otherENamedElement->getName();
                if ( name == otherName )
                    ++count;
            }
        }

        std::stringstream s;
        s << name.empty() ? "%" : uriEscape(name);
        if (count > 0)
            s << "." << count;
        return s.str();
    }
    
    auto eAnnotation = std::dynamic_pointer_cast<EAnnotation>(eObject);
    if (eAnnotation) {
        auto source = eAnnotation->getSource();
        auto contents = eContents();
        int count = 0;
        for (auto otherEObject : *contents) {
            if (otherEObject == eObject)
                break;
            auto otherEAnnotation = std::dynamic_pointer_cast<EAnnotation>(otherEObject);
            if (otherEAnnotation) {
                auto otherSource = otherEAnnotation->getSource();
                if (source == otherSource)
                    ++count;
            }
        }

        std::stringstream s;
        s << '%';
        s << source.empty() ? "%" : uriEscape(source);
        s << '%';
        if (count > 0)
            s << "." << count;
        return s.str();
    }
    return EModelElementImpl::eURIFragmentSegment(feature, eObject);
}
