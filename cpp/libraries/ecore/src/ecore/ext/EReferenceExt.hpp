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

        virtual std::shared_ptr<ecore::EClass> getEReferenceType() const;

        virtual void setEType(const std::shared_ptr<ecore::EClassifier>& newEType);

    protected:
        virtual std::shared_ptr<ecore::EClass> basicGetEReferenceType() const;

    private:
        mutable std::shared_ptr<ecore::EClass> eReferenceType_;
    };

}

#endif /* ECORE_EREFERENCE_EREFERENCEEXT_HPP */