// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLHANDLER_HPP_
#define ECORE_XMLHANDLER_HPP_

#include <xercesc/sax2/DefaultHandler.hpp>

namespace ecore::impl
{
    class XmlHandler : public xercesc::DefaultHandler
    {
    public:
        XmlHandler();

        virtual ~XmlHandler();
    };
}

#endif