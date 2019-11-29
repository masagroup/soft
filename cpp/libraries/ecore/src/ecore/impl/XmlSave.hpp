// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLSAVE_HPP_
#define ECORE_XMLSAVE_HPP_

#include "ecore/impl/XmlNamespaces.hpp"

namespace ecore::impl
{
    class XmlResource;

    class XmlSave
    {
    public:
        XmlSave(XmlResource& resource);

        virtual ~XmlSave();

    private:
        XmlResource& resource_;
        XmlNamespaces namespaces_;
    };
}

#endif