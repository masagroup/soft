#include "ecore/impl/StringUtils.hpp"
#include "ecore/impl/XMILoad.hpp"

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

XMILoad::XMILoad( XMIResource& resource )
    : AbstractXMILoad( resource )
{
    using namespace utf8;
    notFeatures_.insert( NOT_FEATURES.begin(), NOT_FEATURES.end() );
}

XMILoad::~XMILoad()
{
}
