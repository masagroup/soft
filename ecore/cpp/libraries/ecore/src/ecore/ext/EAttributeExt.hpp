// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EATTRIBUTE_EATTRIBUTEEXT_HPP
#define ECORE_EATTRIBUTE_EATTRIBUTEEXT_HPP

#include "ecore/impl/EAttributeImpl.hpp"
#include "ecore/impl/Lazy.hpp"

namespace ecore::ext
{
    class EAttributeExt : public virtual impl::EAttributeImpl
    {
    public:
        virtual ~EAttributeExt();

        virtual std::shared_ptr<ecore::EDataType> getEAttributeType() const;

    protected:
        friend class impl::EcoreFactoryImpl;
        EAttributeExt();

        //*********************************
        // Attributes
        //*********************************
        virtual void setID( bool newID );

        virtual std::shared_ptr<ecore::EDataType> basicGetEAttributeType() const; 
    
    private:
        EAttributeExt& operator=( EAttributeExt const& ) = delete;

        std::shared_ptr<EDataType> initAttributeType();

    private:
        impl::Lazy<std::shared_ptr<EDataType>> attributeType_;
    };
}
#endif /* ECORE_EATTRIBUTE_EATTRIBUTEEXT_HPP */
