// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EMODELELEMENT_EMODELELEMENTEXT_HPP
#define ECORE_EMODELELEMENT_EMODELELEMENTEXT_HPP

#include "ecore/impl/EModelElementImpl.hpp"

namespace ecore::ext
{
    class EModelElementExt : public virtual impl::EModelElementImpl
    {
    private:
        EModelElementExt& operator=( EModelElementExt const& ) = delete;

    protected:
        EModelElementExt();

    public:
        virtual ~EModelElementExt();

        //*********************************
        // Operations
        //*********************************
        virtual std::shared_ptr<ecore::EAnnotation> getEAnnotation( const std::string& source );

        virtual std::shared_ptr<EObject>eObjectForFragmentSegment(const std::string& uriSegment) const;
        virtual std::string eURIFragmentSegment(const std::shared_ptr<EStructuralFeature>& feature, const std::shared_ptr<EObject>& eObject) const;
    
    };
}

#endif // ECORE_EMODELELEMENT_EMODELELEMENTEXT_HPP