// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLRESOURCEFACTORY_HPP_
#define ECORE_XMLRESOURCEFACTORY_HPP_

#include "ecore/Exports.hpp"
#include "ecore/EResourceFactory.hpp"

namespace ecore::impl
{
    class ECORE_API XMLResourceFactory : public EResourceFactory
    {
    public:
        XMLResourceFactory();

        virtual ~XMLResourceFactory();

        virtual std::shared_ptr<EResource> createResource( const URI & uri ) const override;

    };

} // namespace ecore::impl

#endif
