// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLRESOURCE_HPP_
#define ECORE_XMLRESOURCE_HPP_

#include "ecore/Exports.hpp"
#include "ecore/impl/AbstractResource.hpp"
#include "ecore/URI.hpp"

namespace ecore::impl
{
    class ECORE_API XMLResource : public AbstractResource
    {
    public:
        XMLResource();

        XMLResource(const URI& uri);

        virtual ~XMLResource();

    protected:
        // Inherited via AbstractResource
        virtual void doLoad( std::istream & is ) override;

        virtual void doSave( std::ostream & os ) override;
    };

} // namespace ecore::impl

#endif
