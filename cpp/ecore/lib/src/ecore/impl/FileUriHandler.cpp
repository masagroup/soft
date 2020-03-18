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
    auto is = std::make_unique<std::ifstream>();
    is->exceptions( std::ios_base::failbit | std::ios_base::badbit );
    is->open( uri.getPath() );
    return std::move( is );
}

std::unique_ptr<std::ostream> FileURIHandler::createOutputStream( const URI& uri ) const
{
    auto os = std::make_unique<std::ofstream>();
    os->exceptions( std::ios_base::failbit | std::ios_base::badbit );
    os->open( uri.getPath() );
    return std::move( os );
}
