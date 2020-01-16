#include "ecore/impl/XMLString.hpp"

#include <sstream>
#include <limits>

using namespace ecore;
using namespace ecore::impl;

struct XMLString::Segment {
    int lineWidth_;
    std::string buffer_;
};

XMLString::XMLString()
    : lineWidth_( std::numeric_limits<int>::max() )
    , depth_(0)
    , lastElementIsStart_(false)
    , currentSegment_( std::make_shared<Segment>() )
    , indentation_("    ")
    , indents_({""})
{
    segments_ = { currentSegment_ };
}

void XMLString::write(std::ostream& os)
{
    for (auto s : segments_)
        os << s->buffer_;
}

void XMLString::add(const std::string& s)
{
    if (lineWidth_ != std::numeric_limits<int>::max())
        currentSegment_->lineWidth_ += int(s.length());
    currentSegment_->buffer_ += s;
}

void XMLString::addLine()
{
    add("\n");
    currentSegment_->lineWidth_ = 0;
}

void XMLString::startElement(const std::string& name)
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

void XMLString::closeStartElement()
{
    add(">");
    addLine();
    lastElementIsStart_ = false;
}

void XMLString::endElement()
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

void XMLString::endEmptyElement()
{
    removeLast();
    add("/>");
    addLine();
    lastElementIsStart_ = false;
}

void XMLString::addAttribute(const std::string& name, const std::string& value)
{
    startAttribute(name);
    addAttributeContent(value);
    endAttribute();
}

void XMLString::startAttribute(const std::string& name)
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

void XMLString::addAttributeContent(const std::string& content)
{
    add(content);
}

void XMLString::endAttribute()
{
    add("\"");
}

void XMLString::addNil(const std::string& name)
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

void XMLString::addContent(const std::string& name, const std::string& content)
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

std::shared_ptr<XMLString::Segment> XMLString::mark()
{
    auto r = currentSegment_;
    currentSegment_ = std::make_shared<Segment>();
    segments_.push_back(currentSegment_);
    return r;
}

void XMLString::resetToMark(const std::shared_ptr<Segment>& m)
{
    if (m)
        currentSegment_ = m;
}

std::string XMLString::removeLast()
{
    auto end = elementNames_.back();
    elementNames_.pop_back();
    if (end != "") {
        depth_--;
    }
    return end;
}

std::string XMLString::getElementIndent()
{
    return getElementIndent(0);
}

std::string XMLString::getElementIndent(int extra)
{
    auto nesting = depth_ + extra - 1;
    for (auto i = indents_.size() - 1; i < nesting; i++){
        indents_.push_back(indents_[i] + "  ");
    }
    return indents_[nesting];
}

std::string XMLString::getAttributeIndent()
{
    auto nesting = depth_ + + 1;
    for (auto i = indents_.size() - 1; i < nesting; i++) {
        indents_.push_back(indents_[i] + "  ");
    }
    return indents_[nesting];
}
