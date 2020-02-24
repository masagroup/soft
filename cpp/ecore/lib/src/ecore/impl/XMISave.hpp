// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMISAVE_HPP_
#define ECORE_XMISAVE_HPP_

#include "ecore/impl/XMLSave.hpp"

namespace ecore::impl
{
    class XMIResource;

    class XMISave : public XMLSave
    {
    public:
        XMISave( XMIResource& resource );

        virtual ~XMISave();

    protected:
        virtual void saveNamespaces();

    private:

        XMIResource& resource_;
    };
} // namespace ecore::impl

#endif
