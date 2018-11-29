// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECLASS_ECLASSEXT_HPP
#define ECORE_ECLASS_ECLASSEXT_HPP

#include "ecore/impl/EClassImpl.hpp"

namespace ecore::ext
{
    class EClassExt : public impl::EClassImpl
    {
    private:
        EClassExt& operator=( EClassExt const& ) = delete;

    protected:
        friend class EcoreFactoryExt;
        EClassExt();
    
    public:
        virtual ~EClassExt();
    };
}
#endif /* ECORE_ECLASS_ECLASSEXT_HPP */
