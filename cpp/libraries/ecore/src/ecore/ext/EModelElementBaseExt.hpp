// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EMODELELEMENTBASEEXT_HPP
#define ECORE_EXT_EMODELELEMENTBASEEXT_HPP

#include "ecore/impl/EModelElementImpl.hpp"

namespace ecore::ext
{
    template <typename... I>
    class EModelElementBaseExt : public ecore::impl::EModelElementImpl
    {
    private:
        EModelElementBaseExt& operator=( EModelElementBaseExt const& ) = delete;

    protected:
        EModelElementBaseExt();

    public:
        virtual ~EModelElementBaseExt();

        //*********************************
        // Operations
        //*********************************
        virtual std::shared_ptr<ecore::EAnnotation> getEAnnotation( const std::string& source );

        virtual std::shared_ptr<EObject>eObjectForFragmentSegment(const std::string& uriSegment) const;
        virtual std::string eURIFragmentSegment(const std::shared_ptr<EStructuralFeature>& feature, const std::shared_ptr<EObject>& eObject) const;
    };
}

#include "ecore/ext/EModelElementBaseExt.inl"

#endif // ECORE_EXT_EMODELELEMENTBASEEXT_HPP