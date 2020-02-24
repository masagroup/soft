// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EATTRIBUTEBASEEXT_HPP
#define ECORE_EXT_EATTRIBUTEBASEEXT_HPP

#include "ecore/impl/EAttributeBase.hpp"
#include "ecore/impl/Lazy.hpp"

namespace ecore::ext
{
    template <typename... I>
    class EAttributeBaseExt : public ecore::impl::EAttributeBase<I...>
    {
    public:
        virtual ~EAttributeBaseExt();

        virtual std::shared_ptr<ecore::EDataType> getEAttributeType() const;

    protected:
        template<typename... I> friend class impl::EcoreFactoryBase;
        EAttributeBaseExt();

        //*********************************
        // Attributes
        //*********************************
        virtual void setID( bool newID );

        virtual std::shared_ptr<ecore::EDataType> basicGetEAttributeType() const; 
    
    private:
        EAttributeBaseExt& operator=( EAttributeBaseExt const& ) = delete;

        std::shared_ptr<EDataType> initAttributeType();

    private:
        ecore::impl::Lazy<std::shared_ptr<EDataType>> attributeType_;
    };
}

#include "ecore/ext/EAttributeBaseExt.inl"

#endif /* ECORE_EATTRIBUTE_EATTRIBUTEEXT_HPP */
