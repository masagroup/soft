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

#include "ecore/impl/XMLLoad.hpp"

namespace ecore::impl
{
    class XMIResource;

    class XMILoad : public XMLLoad
    {
    public:
        XMILoad( XMIResource& resource );

        virtual ~XMILoad();
    
    protected:
        virtual std::string getXSIType() const;

        virtual void handleAttributes( const std::shared_ptr<EObject>& eObject ); 

    private:
        XMIResource& resource_;
    };
} // namespace ecore::impl

#endif
