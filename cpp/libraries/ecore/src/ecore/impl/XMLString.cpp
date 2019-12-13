#include "ecore/impl/XmlString.hpp"

#include <sstream>
#include <limits>

using namespace ecore;
using namespace ecore::impl;

struct XmlString::Segment {
    int lineWidth_;
    std::string buffer_;
};

XmlString::XmlString()
    : lineWidth_( std::numeric_limits<int>::max() )
    , depth_(0)
    , lastElementIsStart_(false)
    , currentSegment_( std::make_shared<Segment>() )
    , indentation_("    ")
    , indents_({""})
{
    segments_ = { currentSegment_ };
}

void XmlString::write(std::ostream& os)
{
    for (auto s : segments_)
        os << s->buffer_;
}

void XmlString::add(const std::string& s)
{
    if (lineWidth_ != std::numeric_limits<int>::max())
        currentSegment_->lineWidth_ += int(s.length());
    currentSegment_->buffer_ += s;
}

void XmlString::addLine()
{
    add("\n");
    currentSegment_->lineWidth_ = 0;
}

void XmlString::startElement(const std::string& name)
{
    if (lastElementIsStart_)
        closeStartElement();
    elementNames_.push_back(name);
    if (name.length() > 0) {
        depth_++;
        add(getElementIndent());
        add("<");
        add(name);
        lastElementIsStart_ = true;
    } else {
        add(getElementIndent(1));
    }
}

void XmlString::closeStartElement()
{
    add(">");
    addLine();
    lastElementIsStart_ = false;
}

void XmlString::endElement()
{
    if ( lastElementIsStart_ )
        endEmptyElement();
    else {
        auto name = removeLast();
        if (!name.empty() ) {
            add(getElementIndent(1));
            add("</");
            add(name);
            add(">");
            addLine();
        }
    }
}

void XmlString::endEmptyElement()
{
    removeLast();
    add("/>");
    addLine();
    lastElementIsStart_ = false;
}

void XmlString::addAttribute(const std::string& name, const std::string& value)
{
    startAttribute(name);
    addAttributeContent(value);
    endAttribute();
}

void XmlString::startAttribute(const std::string& name)
{
    if (currentSegment_->lineWidth_ > lineWidth_ ){
        addLine();
        add(getAttributeIndent());
    }
    else {
        add(" ");
    }
    add(name);
    add("=\"");
}

void XmlString::addAttributeContent(const std::string& content)
{
    add(content);
}

void XmlString::endAttribute()
{
    add("\"");
}

void XmlString::addNil(const std::string& name)
{
    if (lastElementIsStart_)
        closeStartElement();
    depth_++;
    add(getElementIndent());
    add("<");
    add(name);
    if (currentSegment_->lineWidth_ > lineWidth_) {
        addLine();
        add(getAttributeIndent());
    }
    else
        add(" ");

    add("xsi:nil=\"true\"/>");
    depth_--;
    addLine();
    lastElementIsStart_ = false;
}

void XmlString::addContent(const std::string& name, const std::string& content)
{
    if (lastElementIsStart_)
        closeStartElement();
    depth_++;
    add(getElementIndent());
    add("<");
    add(name);
    add(">");
    add(content);
    add("</");
    depth_--;
    add(name);
    add(">");
    addLine();
    lastElementIsStart_ = false;
}

std::shared_ptr<XmlString::Segment> XmlString::mark()
{
    auto r = currentSegment_;
    currentSegment_ = std::make_shared<Segment>();
    segments_.push_back(currentSegment_);
    return r;
}

void XmlString::resetToMark(const std::shared_ptr<Segment>& m)
{
    if (m)
        currentSegment_ = m;
}

std::string XmlString::removeLast()
{
    auto end = elementNames_.back();
    elementNames_.pop_back();
    if (end != "") {
        depth_--;
    }
    return end;
}

std::string XmlString::getElementIndent()
{
    return getElementIndent(0);
}

std::string XmlString::getElementIndent(int extra)
{
    auto nesting = depth_ + extra - 1;
    for (auto i = indents_.size() - 1; i < nesting; i++){
        indents_.push_back(indents_[i] + "  ");
    }
    return indents_[nesting];
}

std::string XmlString::getAttributeIndent()
{
    auto nesting = depth_ + + 1;
    for (auto i = indents_.size() - 1; i < nesting; i++) {
        indents_.push_back(indents_[i] + "  ");
    }
    return indents_[nesting];
}
