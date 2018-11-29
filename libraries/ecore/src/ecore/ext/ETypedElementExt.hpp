// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************
#ifndef ECORE_ETYPEDELEMENT_ETYPEDELEMENTEXT_HPP
#define ECORE_ETYPEDELEMENT_ETYPEDELEMENTEXT_HPP

#include "ecore/impl/ETypedElementImpl.hpp"


namespace ecore::ext
{
    class ETypedElementExt : public virtual impl::ETypedElementImpl
    {
    private:
        ETypedElementExt& operator=( ETypedElementExt const& ) = delete;

    protected:
        ETypedElementExt();

    public:
        virtual ~ETypedElementExt();


        //*********************************
        // Attributes
        //*********************************
        virtual bool isMany() const;

        virtual bool isRequired() const;
    };

}

#endif // ECORE_ETYPEDELEMENT_ETYPEDELEMENTEXT_HPP