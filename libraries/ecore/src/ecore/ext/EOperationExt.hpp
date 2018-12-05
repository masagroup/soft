// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOPERATIONEXT_HPP
#define ECORE_EOPERATIONEXT_HPP

#include "ecore/impl/EOperationImpl.hpp"


namespace ecore::ext
{

    class EOperationExt : public impl::EOperationImpl
    {
    private:
        EOperationExt( EOperationExt const& ) = delete;
        EOperationExt& operator=( EOperationExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EOperationExt();
    
    public:
        virtual ~EOperationExt();

        virtual bool isOverrideOf( const std::shared_ptr<ecore::EOperation>& someOperation );
    };

}

#endif
