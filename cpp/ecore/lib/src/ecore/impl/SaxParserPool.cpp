#include "ecore/impl/SaxParserPool.hpp"
#include <stdexcept>
#include <xercesc/sax2/XMLReaderFactory.hpp>

using namespace ecore;
using namespace ecore::impl;
using namespace xercesc;

SaxParserPool& SaxParserPool::getInstance()
{
    static SaxParserPool instance;
    return instance;
}

SaxParserPool::SaxParserPool()
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch( const XMLException& toCatch )
    {
        char* message = XMLString::transcode( toCatch.getMessage() );
        std::string msg = message;
        XMLString::release( &message );
        throw std::runtime_error( msg );
    }
}

SaxParserPool::~SaxParserPool()
{
    readers_.clear();

    try
    {
        XMLPlatformUtils::Terminate();
    }
    catch( const XMLException& )
    {
    }
}

SaxParserPool::SaxParser::SaxParser( SaxParserPool& pool )
    : pool_( pool )
{
    auto& readers = pool_.readers_;
    if( readers.empty() )
        reader_ = std::shared_ptr<SAX2XMLReader>( XMLReaderFactory::createXMLReader() );
    else
    {
        reader_ = readers.front();
        readers.pop_front();
    }
}

SaxParserPool::SaxParser::~SaxParser()
{
    auto& readers = pool_.readers_;
    readers.push_back( reader_ );
    reader_.reset();
}

xercesc::SAX2XMLReader& SaxParserPool::SaxParser::getReader() const
{
    return *reader_;
}

std::unique_ptr<SaxParserPool::SaxParser> SaxParserPool::getParser( const std::map<std::string, bool>& features )
{
    auto parser = std::make_unique<SaxParser>( *this );
    for( auto feature : features )
    {
        XMLCh* featureKey = XMLString::transcode( feature.first.c_str() );
        bool featureValue = feature.second;
        parser->getReader().setFeature( featureKey, featureValue );
        XMLString::release( &featureKey );
    }
    return parser;
}
