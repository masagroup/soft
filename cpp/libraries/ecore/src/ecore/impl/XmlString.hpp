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

        void write(std::ostream& os);

        void add(const std::string& s);
        void addLine();

        void startElement(const std::string& name);
        void closeStartElement();
        void endElement();
        void endEmptyElement();

        void addAttribute(const std::string& name, const std::string& value);
        void startAttribute(const std::string& name);
        void addAttributeContent(const std::string& content);
        void endAttribute();

        void addNil(const std::string& name);
        void addContent(const std::string& name, const std::string& content);

        std::shared_ptr<Segment> mark();
        void resetToMark(const std::shared_ptr<Segment>& m);

    private:
        std::string removeLast();

        std::string getElementIndent();
        std::string getElementIndent(int extra);
        std::string getAttributeIndent();

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