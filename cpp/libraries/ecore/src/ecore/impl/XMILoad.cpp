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
    static constexpr char16_t* VERSION_ATTRIB = u"xmi:version";

} // namespace utf16

XMILoad::XMILoad( XMIResource& resource )
    : AbstractXMILoad( resource )
    , resource_( resource )
{
    using namespace utf8;
    notFeatures_.insert( NOT_FEATURES.begin(), NOT_FEATURES.end() );
}

XMILoad::~XMILoad()
{
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
    AbstractXMILoad::handleAttributes( eObject );
}
