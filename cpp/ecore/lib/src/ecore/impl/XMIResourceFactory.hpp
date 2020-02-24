// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMIRESOURCEFACTORY_HPP_
#define ECORE_XMIRESOURCEFACTORY_HPP_

#include "ecore/EResourceFactory.hpp"
#include "ecore/Exports.hpp"

namespace ecore::impl
{
    class ECORE_API XMIResourceFactory : public EResourceFactory
    {
    public:
        XMIResourceFactory();

        virtual ~XMIResourceFactory();

        virtual std::shared_ptr<EResource> createResource( const URI& uri ) const override;
    };

} // namespace ecore::impl

#endif
