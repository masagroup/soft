// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_RESOURCEFACTORYREGISTRY_HPP_
#define ECORE_RESOURCEFACTORYREGISTRY_HPP_

#include "ecore/Exports.hpp"
#include "ecore/EResourceFactoryRegistry.hpp"

namespace ecore::impl
{
    class ECORE_API ResourceFactoryRegistry : public EResourceFactoryRegistry
    {
    public:
        ResourceFactoryRegistry();

        virtual ~ResourceFactoryRegistry();

        virtual std::shared_ptr<EResourceFactory> getFactory( const URI& uri ) const;

        virtual FactoryMap& getProtocolToFactoryMap();

        virtual FactoryMap& getExtensionToFactoryMap();

    private:
        FactoryMap protocolToFactory_;
        FactoryMap extensionToFactory_;
    };

} // namespace ecore::impl

#endif // ECORE_RESOURCEFACTORYREGISTRY_HPP_
