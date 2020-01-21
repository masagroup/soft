// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_EOPERATIONBASEEXT_HPP
#define ECORE_EXT_EOPERATIONBASEEXT_HPP

#include "ecore/impl/EOperationBase.hpp"


namespace ecore::ext
{
    template <typename... I>
    class EOperationBaseExt : public ecore::impl::EOperationBase<I...>
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

}

#include "ecore/ext/EOperationBaseExt.inl"

#endif // ECORE_EXT_EOPERATIONBASEEXT_HPP
