#include "ecore/impl/FileURIHandler.hpp"
#include "ecore/URI.hpp"
#include <fstream>
#include <string.h>

using namespace ecore;
using namespace ecore::impl;

namespace
{
    const char SCHEME_FILE[] = "file";
} // namespace

FileURIHandler::FileURIHandler()
{
}

FileURIHandler::~FileURIHandler()
{
}

bool FileURIHandler::canHandle( const URI& uri ) const
{
    return uri.getScheme() == SCHEME_FILE || ( uri.getScheme().empty() && uri.getAuthority().empty() && uri.getQuery().empty() );
}

std::unique_ptr<std::istream> FileURIHandler::createInputStream( const URI& uri ) const
{
    auto is = std::make_unique<std::ifstream>( uri.getPath() );
    if( is->fail() || is->bad() )
    {
        char buff[256];
        strerror_s( buff, 256, errno );
        throw std::runtime_error( buff );
    }
    return std::move( is );
}

std::unique_ptr<std::ostream> FileURIHandler::createOutputStream( const URI& uri ) const
{
    return std::move( std::make_unique<std::ofstream>( uri.getPath() ) );
}
