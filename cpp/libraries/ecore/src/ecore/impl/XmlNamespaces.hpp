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

#include "ecore/Exports.hpp"
#include <string>
#include <vector>

namespace ecore::impl
{
    class ECORE_API XmlNamespaces
    {
    public:
        void pushContext();

        std::vector<std::pair<std::string, std::string>> popContext();

         /**
            @param prefix prefix to declare
            @param uri uri that maps to the prefix
            @return true if the prefix existed in the current context and
            its uri has been remapped; false if prefix does not exist in the
            current context
         */
        bool declarePrefix( const std::string& prefix, const std::string& uri );

        std::string getPrefix( const std::string& uri );

        std::string getURI( const std::string& prefix );

    private:
        std::vector<std::pair<std::string, std::string>> namespaces_{16};
        int namespacesSize_{0};
        std::vector<int> contexts_{8};
        int currentContext_{0};
    };

} // namespace ecore::impl

#endif