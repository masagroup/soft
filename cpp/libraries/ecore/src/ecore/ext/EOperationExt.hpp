// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EOPERATIONEXT_HPP
#define ECORE_EXT_EOPERATIONEXT_HPP

#include "ecore/impl/EOperationImpl.hpp"


namespace ecore::ext
{
    template <typename... I>
    class EOperationBaseExt : public ecore::impl::EOperationImpl
    {
    private:
        EOperationBaseExt( EOperationBaseExt const& ) = delete;
        EOperationBaseExt& operator=( EOperationBaseExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EOperationBaseExt();
    
    public:
        virtual ~EOperationBaseExt();

        virtual bool isOverrideOf( const std::shared_ptr<ecore::EOperation>& someOperation );
    };

    typedef EOperationBaseExt<EOperation> EOperationExt;

}

#include "ecore/ext/EOperationExt.inl"

#endif
