// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLLOAD_HPP_
#define ECORE_XMLLOAD_HPP_

#include "ecore/impl/AbstractXMLLoad.hpp"

namespace ecore::impl
{
    class XMLLoad : public AbstractXMLLoad
    {
    public:
        XMLLoad( XMLResource& resource );

        virtual ~XMLLoad();

        
    };
     
} // namespace ecore::impl

#endif