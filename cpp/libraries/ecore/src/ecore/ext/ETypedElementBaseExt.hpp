// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ETYPEDELEMENTBASEEXT_HPP
#define ECORE_EXT_ETYPEDELEMENTBASEEXT_HPP

#include "ecore/impl/ETypedElementBase.hpp"


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
}

#include "ecore/ext/ETypedElementBaseExt.inl"

#endif // ECORE_EXT_ETYPEDELEMENTBASEEXT_HPP