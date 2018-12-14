// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EATTRIBUTE_EATTRIBUTEEXT_HPP
#define ECORE_EATTRIBUTE_EATTRIBUTEEXT_HPP

#include "ecore/impl/EAttributeImpl.hpp"

namespace ecore::ext
{
    class EAttributeExt : public virtual impl::EAttributeImpl
    {
    private:
        EAttributeExt& operator=( EAttributeExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EAttributeExt();

        //*********************************
        // Attributes
        //*********************************
        virtual void setID( bool newID );

    public:
        virtual ~EAttributeExt();
    };
}
#endif /* ECORE_EATTRIBUTE_EATTRIBUTEEXT_HPP */
