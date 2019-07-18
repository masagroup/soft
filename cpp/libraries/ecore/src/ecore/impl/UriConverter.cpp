#include "ecore/impl/UriConverter.hpp"
#include "ecore/EUriHandler.hpp"
#include "ecore/impl/ImmutableEList.hpp"
#include "ecore/impl/FileUriHandler.hpp"

using namespace ecore;
using namespace ecore::impl;

UriConverter::UriConverter()
    : uriHandlers_( new ImmutableEList<std::shared_ptr<EUriHandler>>( { std::make_shared<FileUriHandler>() } ) )
{
}

UriConverter::~UriConverter()
{
}

std::unique_ptr<std::istream> UriConverter::createInputStream( const Uri& uri ) const
{
    return std::move( getUriHandler( uri )->createInputStream( uri ) );
}

std::unique_ptr<std::ostream> UriConverter::createOutputStream( const Uri& uri ) const
{
    return std::move( getUriHandler( uri )->createOutputStream( uri ) );
}

std::shared_ptr<EUriHandler> UriConverter::getUriHandler( const Uri& uri ) const
{
    for( auto handler : *uriHandlers_ )
    {
        if( handler->canHandle( uri ) )
            return handler;
    }
    return std::shared_ptr<EUriHandler>();
}

std::shared_ptr<const EList<std::shared_ptr<EUriHandler>>> UriConverter::getUriHandlers() const
{
    return uriHandlers_;
}
