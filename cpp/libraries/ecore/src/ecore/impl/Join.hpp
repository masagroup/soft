// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_IMPL_JOIN_HPP_
#define ECORE_IMPL_JOIN_HPP_

#include <string>

namespace ecore::impl
{
    template < typename Range >
    std::string join(const Range& range, const std::string& sep) {
        std::string s;
        auto end = std::end(range);
        for (auto it = std::begin(range); it != end;  ++it) {
            s += *it;
            if (it != end - 1)
                s += sep;
        }
        return s;
    }
}

#endif // ECORE_LAZY_HPP_
