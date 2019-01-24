#include "ecore/impl/FileUriHandler.hpp"
#include "ecore/Uri.hpp"

using namespace ecore;
using namespace ecore::impl;

namespace
{
    const char SCHEME_FILE[] = "file";

    class FileInputStream : public EUriInputStream
    {
    public:
        FileInputStream()
        {
        
        }

        virtual ~FileInputStream()
        {

        }

        virtual void reset()
        {

        }

        virtual std::size_t getSize()
        {
            return 0;
        }

        virtual std::size_t getPosition()
        {
            return 0;
        }

        virtual std::size_t read( const char* bytes, const std::size_t size )
        {
            return 0;
        }
    };

    class FileOutputStream : public EUriOutputStream
    {
    public:
        FileOutputStream()
        {
        }

        virtual ~FileOutputStream()
        {
        }

        virtual void write( const char* bytes, const std::size_t size )
        {

        }

        virtual void flush()
        {

        }
    };

} // namespace

FileUriHandler::FileUriHandler()
{
}

FileUriHandler::~FileUriHandler()
{
}

bool FileUriHandler::canHandle( const Uri& uri ) const
{
    return uri.getScheme() == SCHEME_FILE
           || ( uri.getScheme().empty() && uri.getAuthority().empty() && uri.getQuery().empty() );
}

std::unique_ptr<EUriInputStream> FileUriHandler::createInputStream( const Uri& uri ) const
{
    return std::move( std::make_unique<FileInputStream>() );
}

std::unique_ptr<EUriOutputStream> FileUriHandler::createOutputStream( const Uri& uri ) const
{
    return std::move( std::make_unique<FileOutputStream>() );
}
