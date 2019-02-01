#include "ecore/impl/NamespaceSupport.hpp"

using namespace ecore;
using namespace ecore::impl;

void NamespaceSupport::pushContext()
{
    if( currentContext_ + 1 == contexts_.size() )
        contexts_.resize( contexts_.size() * 2 );
    contexts_[++currentContext_] = prefixesSize_;
}

std::vector<std::pair<std::u16string, std::u16string>> NamespaceSupport::popContext()
{
    int oldPrefixSize = prefixesSize_;
    prefixesSize_ = contexts_[currentContext_--];
    return std::vector<std::pair<std::u16string, std::u16string>>( prefixes_.begin() + prefixesSize_, prefixes_.begin() + oldPrefixSize );
}

bool NamespaceSupport::declarePrefix( const std::u16string& prefix, const std::u16string& uri )
{
    for( int i = prefixesSize_; i > contexts_[currentContext_]; --i )
    {
        auto& p = prefixes_[i];
        if( p.first == prefix )
        {
            p.second = uri;
            return true;
        }
    }
    if( prefixesSize_ + 1 == prefixes_.size() )
        prefixes_.resize( prefixes_.size() * 2 );
    prefixes_[prefixesSize_++] = std::make_pair( prefix, uri );
    return false;
}

std::u16string NamespaceSupport::getPrefix( const std::u16string& uri )
{
    for( int i = prefixesSize_; i > 0; --i )
    {
        auto& p = prefixes_[i];
        if( p.second == uri )
            return p.first;
    }
    return std::u16string();
}

std::u16string NamespaceSupport::getUri( const std::u16string& prefix )
{
    for( int i = prefixesSize_; i > 0; --i )
    {
        auto& p = prefixes_[i];
        if( p.first == prefix )
            return p.first;
    }
    return std::u16string();
}