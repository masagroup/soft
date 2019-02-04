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

#include "ecore/impl/XmlNamespaces.hpp"

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include <stack>
#include <unordered_map>
#include <string>

namespace ecore
{
    class EFactory;
}

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

        virtual void endElement( const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname );

        virtual void startPrefixMapping( const XMLCh* const prefix, const XMLCh* const uri );

        virtual void endPrefixMapping( const XMLCh* const prefix );

        virtual void characters( const XMLCh* const chars, const XMLSize_t length );

        virtual void error( const xercesc::SAXParseException& exc );

        virtual void fatalError( const xercesc::SAXParseException& exc );

        virtual void warning( const xercesc::SAXParseException& exc );

    private:
        XmlResource& resource_;
        XmlNamespaces namespaces_;
        bool isPushContext_{false};
        bool isRoot_{false};
        std::unordered_map<std::u16string,std::shared_ptr<EFactory>> prefixesToFactories_;
        std::stack<std::u16string> elements_;
    };
} // namespace ecore::impl

#endif