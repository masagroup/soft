#include "ecore/impl/XMLLoad.hpp"
#include "ecore/impl/StringUtils.hpp"

using namespace ecore;
using namespace ecore::impl;

namespace utf8
{
    static constexpr char* NO_NAMESPACE_SCHEMA_LOCATION = "noNamespaceSchemaLocation";
    static constexpr char* TYPE_ATTRIB = "xsi:type";
    static constexpr char* NIL_ATTRIB = "xsi:nil";
    static constexpr char* SCHEMA_LOCATION_ATTRIB = "xsi:schemaLocation";
    static constexpr char* NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB = "xsi:noNamespaceSchemaLocation";
    static std::unordered_set<std::string> NOT_FEATURES = {TYPE_ATTRIB, SCHEMA_LOCATION_ATTRIB, NO_NAMESPACE_SCHEMA_LOCATION_ATTRIB};
}

XMLLoad::XMLLoad( XMLResource& resource )
    : AbstractXMLLoad( resource )
{
    using namespace utf8;
    notFeatures_.insert( NOT_FEATURES.begin(), NOT_FEATURES.end() );
}

XMLLoad::~XMLLoad()
{
}
