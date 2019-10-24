#include "ecore/impl/ResourceFactoryRegistry.hpp"
#include "ecore/EResourceFactory.hpp"
#include "ecore/URI.hpp"

using namespace ecore;
using namespace ecore::impl;

ResourceFactoryRegistry::ResourceFactoryRegistry()
{
}

ResourceFactoryRegistry::~ResourceFactoryRegistry()
{
}

EResourceFactory* ResourceFactoryRegistry::getFactory( const URI& uri )
{
    {
        auto it = protocolToFactory_.find( uri.getScheme() );
        if( it != protocolToFactory_.end() )
            return it->second;
    }
    {
        std::size_t ndx = uri.getPath().find_last_of( '.' );
        if( ndx != -1 )
        {
            auto extension = uri.getPath().substr( ndx + 1 );
            auto it = extensionToFactory_.find( extension );
            if( it != extensionToFactory_.end() )
                return it->second;
        }
    }
    {
        auto it = extensionToFactory_.find( DEFAULT_EXTENSION );
        if( it != extensionToFactory_.end() )
            return it->second;
    }

    return nullptr;
}

ResourceFactoryRegistry::FactoryMap& ResourceFactoryRegistry::getProtocolToFactoryMap()
{
    return protocolToFactory_;
}

ResourceFactoryRegistry::FactoryMap& ResourceFactoryRegistry::getExtensionToFactoryMap()
{
    return extensionToFactory_;
}
