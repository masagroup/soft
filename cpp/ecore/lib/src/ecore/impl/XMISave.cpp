#include "ecore/impl/XMISave.hpp"
#include "ecore/impl/XMIResource.hpp"

using namespace ecore;
using namespace ecore::impl;

namespace utf8
{
    const char* XMI_VERSION = "xmi:version";
    const char* XMI_NS = "xmlns:xmi";
    const char* XMI_URI = "http://www.omg.org/XMI";
}

XMISave::XMISave( XMIResource& resource )
    : XMLSave( resource )
    , resource_( resource )
{
}

XMISave::~XMISave()
{
}

void XMISave::saveNamespaces()
{
    using namespace utf8;
    str_.addAttribute( XMI_VERSION , resource_.getXMIVersion() );
    str_.addAttribute( XMI_NS, XMI_URI );
    XMLSave::saveNamespaces();
}
