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

#include "ecore/impl/AbstractResource.hpp"
#include "ecore/Uri.hpp"

namespace ecore::impl
{
    class XmlResource : public AbstractResource
    {
    public:
        XmlResource(const Uri& uri);

        virtual ~XmlResource();

    protected:
        // Inherited via AbstractResource
        virtual void doLoad( std::istream & is ) override;

        virtual void doSave( std::ostream & os ) override;
    private:
        Uri uri_;
    };

} // namespace ecore::impl

#endif
