// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EMODELELEMENTBASEEXT_HPP
#error This file may only be included from EModelElementBaseExt.hpp
#endif

#include "ecore/EAnnotation.hpp"
#include "ecore/ENamedElement.hpp"
#include "ecore/EList.hpp"

#include <algorithm>
#include <sstream>

namespace ecore::ext
{
    template <typename... I>
    EModelElementBaseExt<I...>::EModelElementBaseExt()
    {
    }

    template <typename... I>
    EModelElementBaseExt<I...>::~EModelElementBaseExt()
    {
    }

    template <typename... I>
    std::shared_ptr<EAnnotation> EModelElementBaseExt<I...>::getEAnnotation(const std::string& source)
    {
        if (eAnnotations_)
        {
            auto it = std::find_if(eAnnotations_->begin(), eAnnotations_->end(), [=](const auto& a)
                {
                    return source == a->getSource();
                });
            if (it != eAnnotations_->end())
                return *it;
        }
        return std::shared_ptr<EAnnotation>();
    }

    template <typename... I>
    std::shared_ptr<EObject> EModelElementBaseExt<I...>::eObjectForFragmentSegment(const std::string& uriFragmentSegment) const
    {
        if (!uriFragmentSegment.empty()) {
            // Is the first character a special character, i.e., something other than '@'?
            char firstCharacter = uriFragmentSegment[0];
            if (firstCharacter != '@') {
                // Is it the start of a source URI of an annotation?
                if (firstCharacter == '%') {
                    // Find the closing '%' and make sure it's not just the opening '%'
                    auto index = uriFragmentSegment.find_last_of("%");
                    auto hasCount = false;
                    if (index != 0 && (index == uriFragmentSegment.size() - 1 || (hasCount = uriFragmentSegment[index + 1] == '.')))
                    {
                        // Decode all encoded characters.
                        //
                        auto encodedSource = uriFragmentSegment.substr(1, index);
                        auto source = "%" == encodedSource ? "" : uriUnescape(encodedSource);

                        // Check for a count, i.e., a '.' followed by a number.
                        int count = 0;
                        if (hasCount) {
                            try {
                                count = std::stoi(uriFragmentSegment.substr(index + 2));
                            }
                            catch (...) {

                            }
                        }

                        // Look for the annotation with the matching source.
                        auto contents = eContents();
                        for (auto eObject : *contents) {

                            auto eAnnotation = std::dynamic_pointer_cast<EAnnotation>(eObject);
                            if (eAnnotation) {
                                auto otherSource = eAnnotation->getSource();
                                if (source == otherSource && count-- == 0)
                                    return eAnnotation;
                            }

                        }
                        return nullptr;
                    }
                }

                // Look for trailing count.
                auto index = uriFragmentSegment.find_last_of(".");
                auto name = index == -1 ? uriFragmentSegment : uriFragmentSegment.substr(0, index);
                int count = 0;
                if (index != -1) {
                    try {
                        count = std::stoi(uriFragmentSegment.substr(index + 1));
                    }
                    catch (...) {
                        name = uriFragmentSegment;
                    }
                }

                name = name == "%" ? "" : uriUnescape(name);

                auto contents = eContents();
                for (auto eObject : *contents) {

                    auto eNamedElement = std::dynamic_pointer_cast<ENamedElement>(eObject);
                    if (eNamedElement) {
                        auto otherName = eNamedElement->getName();
                        if (name == otherName && count-- == 0)
                            return eNamedElement;
                    }

                }
                return nullptr;
            }
        }
        return EModelElementBase<I...>::eObjectForFragmentSegment(uriFragmentSegment);
    }

    template <typename... I>
    std::string  EModelElementBaseExt<I...>::eURIFragmentSegment(const std::shared_ptr<EStructuralFeature>& feature, const std::shared_ptr<EObject>& eObject) const
    {
        auto eNamedElement = std::dynamic_pointer_cast<ENamedElement>(eObject);
        if (eNamedElement) {
            auto name = eNamedElement->getName();
            auto contents = eContents();
            int count = 0;
            for (auto otherEObject : *contents)
            {
                if (otherEObject == eObject)
                    break;

                auto otherENamedElement = std::dynamic_pointer_cast<ENamedElement>(otherEObject);
                if (otherENamedElement)
                {
                    auto otherName = otherENamedElement->getName();
                    if (name == otherName)
                        ++count;
                }
            }

            std::stringstream s;
            s << (name.empty() ? "%" : uriEscape(name));
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
            s << (source.empty() ? "%" : uriEscape(source));
            s << '%';
            if (count > 0)
                s << "." << count;
            return s.str();
        }
        return EModelElementBase<I...>::eURIFragmentSegment(feature, eObject);
    }

}