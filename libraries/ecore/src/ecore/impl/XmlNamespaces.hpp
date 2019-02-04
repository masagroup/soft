// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_NAMEPSPACESUPPORT_HPP_
#define ECORE_NAMEPSPACESUPPORT_HPP_

#include <string>
#include <vector>

namespace ecore::impl
{
    class XmlNamespaces
    {
    public:
        void pushContext();

        std::vector<std::pair<std::u16string, std::u16string>> popContext();

        bool declarePrefix( const std::u16string& prefix, const std::u16string& uri );

        std::u16string getPrefix( const std::u16string& uri );

        std::u16string getUri( const std::u16string& prefix );

    private:
        std::vector<std::pair<std::u16string, std::u16string>> namespaces_{16};
        int namespacesSize_{0};
        std::vector<int> contexts_{8};
        int currentContext_{-1};
    };

} // namespace ecore::impl

#endif