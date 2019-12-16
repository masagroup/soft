// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMILOAD_HPP_
#define ECORE_XMILOAD_HPP_

#include "ecore/impl/AbstractXMILoad.hpp"

namespace ecore::impl
{
    class XMILoad : public AbstractXMILoad
    {
    public:
        XMILoad( XMLResource& resource );

        virtual ~XMILoad();
    
    };
} // namespace ecore::impl

#endif
