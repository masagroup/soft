#include "ecore/impl/SaxParserPool.hpp"
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <stdexcept>

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
    try
    {
        XMLPlatformUtils::Terminate();
    }
    catch( const XMLException& )
    {
    }
}

std::shared_ptr<SAX2XMLReader> SaxParserPool::getParser( const std::map<std::string, bool>& features )
{
    std::shared_ptr<SAX2XMLReader> result;
    if( parsers_.empty()  )
        result = std::shared_ptr<SAX2XMLReader>( XMLReaderFactory::createXMLReader() );
    else
    {
        result = parsers_.front();
        parsers_.pop_front();
    }
    for (auto feature : features)
    {
        XMLCh* featureKey = XMLString::transcode( feature.first.c_str() );
        bool featureValue = feature.second;
        result->setFeature( featureKey, featureValue );
        XMLString::release( &featureKey );
    }   
    return result;
}

void SaxParserPool::releaseParser( std::shared_ptr<xercesc::SAX2XMLReader>& parser )
{
    parsers_.push_back( parser );
}



