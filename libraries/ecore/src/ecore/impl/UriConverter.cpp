#include "ecore/impl/UriConverter.hpp"
#include "ecore/EUriHandler.hpp"
#include "ecore/impl/ArrayEList.hpp"

using namespace ecore;
using namespace ecore::impl;

UriConverter::UriConverter()
    : uriHandlers_( std::make_shared<ArrayEList<std::shared_ptr<EUriHandler>>>() )
{
}

UriConverter::~UriConverter()
{
}

std::unique_ptr<EUriInputStream> UriConverter::createInputStream( const Uri& uri ) const
{
    return std::move( getUriHandler( uri )->createInputStream( uri ) );
}

std::unique_ptr<EUriOutputStream> UriConverter::createOutputStream( const Uri& uri ) const
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
