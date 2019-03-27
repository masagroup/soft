//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_STREAM_HPP_
#define ECORE_STREAM_HPP_

#include <iostream>
#include <vector>
#include <memory>
#include "ecore/EList.hpp"
#include "ecore/Uri.hpp"

namespace ecore
{
    template<typename C, typename T>
    std::basic_ostream<C, T>& operator<<( std::basic_ostream<C, T>& os, std::nullptr_t )
    {
        return os << (void*) nullptr;
    }

    template<typename C, typename T, typename U>
    std::basic_ostream<C, T>& operator<<( std::basic_ostream<C, T>& os, const std::weak_ptr<U>& ptr )
    {
        return os << ptr.lock();
    }


    template<typename C, typename T, typename Container >
    std::basic_ostream<C, T>& print_container( std::basic_ostream<C, T>& os, const Container& c )
    {
        bool first = true;
        os << "[";
        for (auto it= std::begin(c) ; it != std::end(c) ; ++it )
        {
            os << (first ? "" : ",") << *it;
            first = false;
        }
        os << "]";
        return os;
    }

    template<typename C, typename T, typename U >
    std::basic_ostream<C, T>& operator<<( std::basic_ostream<C, T>& os, const std::vector<U>& v )
    {
        return print_container(os,v);
    }

    template<typename C, typename T, typename U >
    std::basic_ostream<C, T>& operator<<( std::basic_ostream<C, T>& os, const EList<U>& l )
    {
        return print_container( os, l );
    }

    template<typename C, typename T>
    std::basic_ostream<C, T>& operator<<( std::basic_ostream<C, T>& os, const Uri& uri )
    {
        return os << uri.toString();
    }


}

#endif