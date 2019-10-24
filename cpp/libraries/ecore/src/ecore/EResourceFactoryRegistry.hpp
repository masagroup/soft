// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ERESOURCEFACTORYREGISTRY_HPP_
#define ECORE_ERESOURCEFACTORYREGISTRY_HPP_

#include "ecore/Exports.hpp"
#include "ecore/EResourceFactory.hpp"

#include <string>
#include <memory>
#include <unordered_map>

namespace ecore
{
    class URI;

    class ECORE_API EResourceFactoryRegistry
    {
    public:
        static std::shared_ptr<EResourceFactoryRegistry> getInstance();

    public:
        static constexpr const char* DEFAULT_EXTENSION = "*";

        virtual ~EResourceFactoryRegistry() = default;

        using FactoryMap = std::unordered_map<std::string, EResourceFactory*>;

        virtual EResourceFactory* getFactory( const URI& uri ) = 0;

        virtual FactoryMap& getProtocolToFactoryMap() = 0;

        virtual FactoryMap& getExtensionToFactoryMap() = 0;
    };

} // namespace ecore

#endif // ECORE_ERESOURCEFACTORYREGISTRY_HPP_
