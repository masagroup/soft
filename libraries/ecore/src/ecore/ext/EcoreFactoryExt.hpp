// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************
#ifndef ECOREFACTORYEXT_HPP
#define ECOREFACTORYEXT_HPP

#include "ecore/impl/EcoreFactoryImpl.hpp"


namespace ecore::ext
{
    class EcoreFactoryExt : public impl::EcoreFactoryImpl
    {
    private:
        EcoreFactoryExt( EcoreFactoryExt const& ) = delete;
        EcoreFactoryExt& operator=( EcoreFactoryExt const& ) = delete;

    protected:
        friend class EcoreFactory;
        // Constructor
        EcoreFactoryExt();

    public:
        virtual ~EcoreFactoryExt();

        virtual std::shared_ptr<EClass> createEClass() const;
        
    private:
        static EcoreFactoryExt* create();
    };
}
#endif /* ECOREFACTORYEXT_HPP */
