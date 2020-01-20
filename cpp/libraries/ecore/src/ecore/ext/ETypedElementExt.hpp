// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ETYPEDELEMENTEXT_HPP
#define ECORE_EXT_ETYPEDELEMENTEXT_HPP

#include "ecore/impl/ETypedElementImpl.hpp"


namespace ecore::ext
{
    template <typename... I>
    class ETypedElementBaseExt : public ecore::impl::ETypedElementBase<I...>
    {
    private:
        ETypedElementBaseExt& operator=( ETypedElementBaseExt const& ) = delete;

    protected:
        ETypedElementBaseExt();

    public:
        virtual ~ETypedElementBaseExt();


        //*********************************
        // Attributes
        //*********************************
        virtual bool isMany() const;

        virtual bool isRequired() const;
    };

    typedef ETypedElementBaseExt<ETypedElement> ETypedElementExt;

}

#include "ecore/ext/ETypedElementExt.inl"

#endif // ECORE_EXT_ETYPEDELEMENTEXT_HPP