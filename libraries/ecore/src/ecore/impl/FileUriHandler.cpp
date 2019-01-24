#include "ecore/impl/FileUriHandler.hpp"
#include "ecore/Uri.hpp"
#include <fstream>
#include <string.h>

using namespace ecore;
using namespace ecore::impl;

namespace
{
    const char SCHEME_FILE[] = "file";

    class FileInputStream : public EUriInputStream
    {
    public:
        FileInputStream( const Uri& uri )
            : uri_( uri )
            , is_( uri.getPath() )
        {
            if( is_.fail() )
            {
                char errmsg[94];
                strerror_s( errmsg, 94, errno );
                throw std::exception( errmsg );
            }
        }

        virtual ~FileInputStream()
        {
            if( is_.is_open() )
                is_.close();
        }

        virtual void reset()
        {
            is_.seekg( 0 );
        }

        virtual std::size_t read( char* bytes, const std::size_t size )
        {
            if( !is_.good() && !is_.eof() )
                throw std::runtime_error( "unable to read data from file '" + uri_.toString() + "'" );

            is_.read( bytes, size );

            if( is_.eof() )
                return is_.gcount();
            else if( !is_.eof() && is_.fail() )
                throw std::runtime_error( "unable to read data from file '" + uri_.toString() + "'" );
            else
            {
                is_.setstate( std::ios_base::goodbit );
                return size;
            }
        }

    private:
        Uri uri_;
        std::ifstream is_;
    };

    class FileOutputStream : public EUriOutputStream
    {
    public:
        FileOutputStream( const Uri& uri )
            : uri_( uri )
            , os_( uri.getPath() )
        {
            if( os_.fail() )
            {
                char errmsg[94];
                strerror_s( errmsg, 94, errno );
                throw std::runtime_error( errmsg );
            }
        }

        virtual ~FileOutputStream()
        {
            if( os_.is_open() )
                os_.close();
        }

        virtual void write( const char* bytes, const std::size_t size )
        {
            os_.write( bytes, size );
            if( !os_.good() )
                throw std::runtime_error( "unable to write data to file '" + uri_.toString() + "'" );
        }

        virtual void flush()
        {
            os_.flush();
        }

    private:
        Uri uri_;
        std::ofstream os_;
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
    return std::move( std::make_unique<FileInputStream>( uri ) );
}

std::unique_ptr<EUriOutputStream> FileUriHandler::createOutputStream( const Uri& uri ) const
{
    return std::move( std::make_unique<FileOutputStream>( uri ) );
}
