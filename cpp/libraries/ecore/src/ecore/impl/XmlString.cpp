#include "ecore/impl/XmlString.hpp"

#include <sstream>
#include <limits>

using namespace ecore;
using namespace ecore::impl;

struct XmlString::Segment {
    int lineWidth_;
    std::stringstream stream_;
};

XmlString::XmlString()
    : lineWidth_( std::numeric_limits<int>::max() )
    , depth_(0)
    , lastElementIsStart_(false)
    , currentSegment_( std::make_shared<Segment>() )
    , segments_({currentSegment_})
    , indentation_("    ")
    , indents_({""})
{
}
