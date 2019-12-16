#include "ecore/impl/XMILoad.hpp"
#include "ecore/impl/StringUtils.hpp"
#include "ecore/impl/XMIResource.hpp"

using namespace ecore;
using namespace ecore::impl;

namespace utf8
{
    static constexpr char* TYPE_ATTRIB = "xmi:type";
    static constexpr char* ID_ATTRIB = "xmi:id";
    static constexpr char* VERSION_ATTRIB = "xmi:version";
    static constexpr char* UUID_ATTRIB = "xmi:uuid";
    static std::unordered_set<std::string> NOT_FEATURES = {TYPE_ATTRIB, VERSION_ATTRIB, UUID_ATTRIB};
} // namespace utf8

namespace utf16
{
    static constexpr char16_t* XMI_URI = u"http://www.omg.org/XMI";
    static constexpr char16_t* TYPE = u"type";
    static constexpr char16_t* XMI_TYPE_ATTRIB = u"xmi:type";
} // namespace utf16

namespace utf16
{
    static constexpr char16_t* VERSION_ATTRIB = u"xmi:version";

} // namespace utf16

XMILoad::XMILoad( XMIResource& resource )
    : XMLLoad( resource )
    , resource_( resource )
{
    using namespace utf8;
    notFeatures_.insert( NOT_FEATURES.begin(), NOT_FEATURES.end() );
}

XMILoad::~XMILoad()
{
}

std::string ecore::impl::XMILoad::getXSIType() const
{
    using namespace utf16;
    auto xsiType = XMLLoad::getXSIType();
    if( xsiType.empty() )
    {
        auto xmiType = isNamespaceAware_
                           ? ( attributes_ ? attributes_->getValue( XMI_URI, TYPE ) : attributes_->getValue( XMI_TYPE_ATTRIB ) )
                           : nullptr;
        xsiType = xmiType ? utf16_to_utf8( xmiType ) : "";
    }
    return xsiType;
}

void XMILoad::handleAttributes( const std::shared_ptr<EObject>& eObject )
{
    using namespace utf16;
    if( attributes_ )
    {
        auto xmiVersion = attributes_->getValue( VERSION_ATTRIB );
        if( xmiVersion )
            resource_.setXMIVersion( utf16_to_utf8( xmiVersion ) );
    }
    XMLLoad::handleAttributes( eObject );
}
