// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMIRESOURCE_HPP_
#define ECORE_XMIRESOURCE_HPP_

#include "ecore/Exports.hpp"
#include "ecore/URI.hpp"
#include "ecore/impl/XMLResource.hpp"

namespace ecore::impl
{
    class ECORE_API XMIResource : public XMLResource
    {
    public:
        XMIResource();

        XMIResource( const URI& uri );

        virtual ~XMIResource();

    protected:
        virtual std::unique_ptr<AbstractXMLLoad> createXMLLoad() override;
    };

} // namespace ecore::impl

#endif
