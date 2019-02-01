// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLHANDLER_HPP_
#define ECORE_XMLHANDLER_HPP_

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include <stack>
#include <string>
#include <unordered_map>

namespace ecore::impl
{
    class XmlResource;

    class XmlHandler : public xercesc::DefaultHandler
    {
    public:
        XmlHandler( XmlResource& resource );

        virtual ~XmlHandler();

        virtual void startDocument();

        virtual void endDocument();

        virtual void startElement( const XMLCh* const uri,
                                   const XMLCh* const localname,
                                   const XMLCh* const qname,
                                   const xercesc::Attributes& attrs );

        virtual void startPrefixMapping( const XMLCh* const prefix, const XMLCh* const uri );

        virtual void endPrefixMapping( const XMLCh* const prefix );

        virtual void characters( const XMLCh* const chars, const XMLSize_t length );

        virtual void error( const xercesc::SAXParseException& exc );

        virtual void fatalError( const xercesc::SAXParseException& exc );

        virtual void warning( const xercesc::SAXParseException& exc );

    private:
        XmlResource& resource_;
        bool isRoot_;
        std::stack<std::u16string> elements_;
        std::unordered_map<std::u16string, std::u16string> prefixesToNamespaces_;
    };
} // namespace ecore::impl

#endif