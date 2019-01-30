#include "XmlResource.hpp"

using namespace ecore;
using namespace ecore::impl;

XmlResource::XmlResource( const Uri& uri )
    : uri_( uri )
{
}

XmlResource::~XmlResource()
{
}

void XmlResource::doLoad( std::istream& is )
{
}

void XmlResource::doSave( std::ostream& os )
{
}
