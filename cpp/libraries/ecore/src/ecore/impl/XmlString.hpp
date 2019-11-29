// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLSTRING_HPP_
#define ECORE_XMLSTRING_HPP_

#include <vector>
#include <memory>
#include <string>

namespace ecore::impl
{
    class XmlString
    {
    public:
        struct Segment;

    public:
        XmlString();

    private:
        std::vector<std::shared_ptr<Segment>> segments_;
        std::shared_ptr<Segment> currentSegment_;
        int lineWidth_;
        int depth_;
        std::string indentation_;
        std::vector<std::string> indents_;
        bool lastElementIsStart_;
        std::vector<std::string> elementNames_;
    };
}

#endif