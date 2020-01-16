#include "ecore/impl/XMIResource.hpp"
#include "ecore/impl/SaxParserPool.hpp"
#include "ecore/impl/XMILoad.hpp"
#include "ecore/impl/XMLInputSource.hpp"
#include "ecore/impl/XMISave.hpp"

using namespace ecore;
using namespace ecore::impl;

const char* XMI_VERSION = "2.0";

XMIResource::XMIResource()
    : XMLResource()
    , xmiVersion_( XMI_VERSION )
{
}

XMIResource::XMIResource( const URI& uri )
    : XMLResource( uri )
    , xmiVersion_( XMI_VERSION )
{
}

XMIResource::~XMIResource()
{
}

std::string ecore::impl::XMIResource::getXMIVersion() const
{
    return xmiVersion_;
}

void ecore::impl::XMIResource::setXMIVersion( const std::string& version )
{
    xmiVersion_ = version;
}

std::unique_ptr<XMLLoad> XMIResource::createXMLLoad()
{
    return std::move( std::make_unique<XMILoad>( *this ) );
}

std::unique_ptr<XMLSave> XMIResource::createXMLSave()
{
    return std::move( std::make_unique<XMISave>( *this ) );
}
