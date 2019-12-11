#include "ecore/impl/ResourceURIConverter.hpp"
#include "ecore/impl/ImmutableEList.hpp"
#include "ecore/impl/FileURIHandler.hpp"
#include "ecore/URI.hpp"

using namespace ecore;
using namespace ecore::impl;

ResourceURIConverter::ResourceURIConverter()
    : uriHandlers_( new ImmutableEList<std::shared_ptr<URIHandler>>( { std::make_shared<FileURIHandler>() } ) )
{
}

ResourceURIConverter::~ResourceURIConverter()
{
}

std::unique_ptr<std::istream> ResourceURIConverter::createInputStream( const URI& uri ) const
{
    auto uriHandler = getURIHandler(uri);
    return uriHandler ? std::move(uriHandler->createInputStream(uri)) : nullptr;
}

std::unique_ptr<std::ostream> ResourceURIConverter::createOutputStream( const URI& uri ) const
{
    auto uriHandler = getURIHandler(uri);
    return uriHandler ? std::move(uriHandler->createOutputStream(uri)) : nullptr;
}

URI ResourceURIConverter::normalize(const URI& uri) const
{
    return uri;
}

std::shared_ptr<URIHandler> ResourceURIConverter::getURIHandler( const URI& uri ) const
{
    for( auto handler : *uriHandlers_ )
    {
        if( handler->canHandle( uri ) )
            return handler;
    }
    return std::shared_ptr<URIHandler>();
}

std::shared_ptr<const EList<std::shared_ptr<URIHandler>>> ResourceURIConverter::getURIHandlers() const
{
    return uriHandlers_;
}
