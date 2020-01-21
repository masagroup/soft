// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EREFERENCEBASEEXT_HPP
#define ECORE_EXT_EREFERENCEBASEEXT_HPP

#include "ecore/impl/EReferenceBase.hpp"

namespace ecore
{
    class EFactory;
}

namespace ecore::ext
{
    template <typename... I>
    class EReferenceBaseExt : public ecore::impl::EReferenceBase<I...>
    {
    private:
        EReferenceBaseExt& operator=( EReferenceBaseExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EReferenceBaseExt();

    public:
        virtual ~EReferenceBaseExt();

        virtual bool isContainer() const;

        virtual std::shared_ptr<ecore::EClass> getEReferenceType() const;

        virtual void setEType(const std::shared_ptr<ecore::EClassifier>& newEType);

    protected:
        virtual std::shared_ptr<ecore::EClass> basicGetEReferenceType() const;

    private:
        mutable std::shared_ptr<ecore::EClass> eReferenceType_;
    };

}

#include "ecore/ext/EReferenceBaseExt.inl"

#endif /* ECORE_EXT_EREFERENCEBASEEXT_HPP */