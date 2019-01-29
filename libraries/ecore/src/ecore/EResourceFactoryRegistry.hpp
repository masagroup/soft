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

#include <memory>
#include <unordered_map>

namespace ecore
{
    class Uri;

    class EResourceFactory;

    class EResourceFactoryRegistry
    {
    public:
        static constexpr const char* DEFAULT_EXTENSION = "*";

        static EResourceFactoryRegistry& getInstance();

        virtual ~EResourceFactoryRegistry() = default;

        using FactoryMap = std::unordered_map<std::string, std::unique_ptr<EResourceFactory>>;

        virtual EResourceFactory* getFactory( const Uri& uri ) = 0;

        virtual FactoryMap& getProtocolToFactoryMap() = 0;

        virtual FactoryMap& getExtensionToFactoryMap() = 0;
    };

} // namespace ecore

#endif // ECORE_ERESOURCEFACTORYREGISTRY_HPP_
