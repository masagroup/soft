#include "ecore/impl/XmlNamespaces.hpp"

using namespace ecore;
using namespace ecore::impl;

void XmlNamespaces::pushContext()
{
    if( currentContext_ + 1 == contexts_.size() )
        contexts_.resize( contexts_.size() * 2 );
    contexts_[++currentContext_] = namespacesSize_;
}

std::vector<std::pair<std::u16string, std::u16string>> XmlNamespaces::popContext()
{
    int oldPrefixSize = namespacesSize_;
    namespacesSize_ = contexts_[currentContext_--];
    return std::vector<std::pair<std::u16string, std::u16string>>( namespaces_.begin() + namespacesSize_,
                                                                   namespaces_.begin() + oldPrefixSize );
}

bool XmlNamespaces::declarePrefix( const std::u16string& prefix, const std::u16string& uri )
{
    for( int i = namespacesSize_; i > contexts_[currentContext_]; --i )
    {
        auto& p = namespaces_[i - 1];
        if( p.first == prefix )
        {
            p.second = uri;
            return true;
        }
    }
    if( namespacesSize_ + 1 == namespaces_.size() )
        namespaces_.resize( namespaces_.size() * 2 );
    namespaces_[namespacesSize_++] = std::make_pair( prefix, uri );
    return false;
}

std::u16string XmlNamespaces::getPrefix( const std::u16string& uri )
{
    for( int i = namespacesSize_; i > 0; --i )
    {
        auto& p = namespaces_[i - 1];
        if( p.second == uri )
            return p.first;
    }
    return std::u16string();
}

std::u16string XmlNamespaces::getUri( const std::u16string& prefix )
{
    for( int i = namespacesSize_; i > 0; --i )
    {
        auto& p = namespaces_[i - 1];
        if( p.first == prefix )
            return p.second;
    }
    return std::u16string();
}