#include "ecore/impl/AbstractXMILoad.hpp"
#include "ecore/impl/StringUtils.hpp"
#include "ecore/impl/XMIResource.hpp"

using namespace ecore;
using namespace ecore::impl;

namespace utf16
{
    static constexpr char16_t* XMI_URI = u"http://www.omg.org/XMI";
    static constexpr char16_t* TYPE = u"type";
    static constexpr char16_t* XMI_TYPE_ATTRIB = u"xmi:type";
}

AbstractXMILoad::AbstractXMILoad( XMIResource& resource )
    : AbstractXMLLoad( resource )
{
}

AbstractXMILoad::~AbstractXMILoad()
{
}

std::string AbstractXMILoad::getXSIType() const
{
    using namespace utf16;
    auto xsiType = AbstractXMLLoad::getXSIType();
    if( xsiType.empty() )
    {
        auto xmiType = isNamespaceAware_
                           ? ( attributes_ ? attributes_->getValue( XMI_URI, TYPE ) : attributes_->getValue( XMI_TYPE_ATTRIB ) )
                           : nullptr;
        xsiType = xmiType ? utf16_to_utf8( xmiType ) : "";
    }
    return xsiType;
}
