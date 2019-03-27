// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EREFERENCE_EREFERENCEEXT_HPP
#define ECORE_EREFERENCE_EREFERENCEEXT_HPP

#include "ecore/impl/EReferenceImpl.hpp"

namespace ecore
{
    class EFactory;
}

namespace ecore::ext
{
    class EReferenceExt : public virtual impl::EReferenceImpl
    {
    private:
        EReferenceExt& operator=( EReferenceExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EReferenceExt();

    public:
        virtual ~EReferenceExt();

        virtual bool isContainer() const;

    };

}

#endif /* ECORE_EREFERENCE_EREFERENCEEXT_HPP */