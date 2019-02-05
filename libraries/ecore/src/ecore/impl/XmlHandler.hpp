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
#include <string>
#include <unordered_map>

namespace ecore
{
    class EClassifier;
    class EFactory;
    class EObject;
    class EReference;
    class EStructuralFeature;

} // namespace ecore

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

    protected:
        virtual void processElement( const std::u16string& name,
                                     const std::u16string& prefix,
                                     const std::u16string& localName,
                                     const xercesc::Attributes& attrs );
        
        virtual void handleFeature( const std::u16string& prefix, const std::u16string& localName );
        

    private:
        void handleSchemaLocation( const xercesc::Attributes& attrs );
        void handleXSISchemaLocation( const std::u16string& schemaLocation );
        void handleXSINoNamespaceSchemaLocation( const std::u16string& schemaLocation );
        void handleAttributes( const std::shared_ptr<EObject>& eObject, const xercesc::Attributes& attrs );
        void setAttributeValue( const std::shared_ptr<EObject>& eObject, const std::u16string& name, const std::u16string& value );

        std::shared_ptr<EFactory> getFactoryForPrefix( const std::u16string& prefix );
        std::shared_ptr<EStructuralFeature> getFeature( const std::shared_ptr<EObject>& eObject, const std::u16string& name );
        std::shared_ptr<EObject> createObject( const std::u16string& prefix, const std::u16string& localName );
        std::shared_ptr<EObject> createObject( const std::shared_ptr<EFactory>& eFactory, const std::shared_ptr<EClassifier>& type );
        
        enum FeatureKind
        {
            Single = 1,
            Many,
            ManyAdd,
            ManyMove,
            Other
        };

        FeatureKind getFeatureKind( const std::shared_ptr<EStructuralFeature>& eFeature ) const;


        void setFeatureValue( const std::shared_ptr<EObject>& eObject,
                              const std::shared_ptr<EStructuralFeature>& eFeature,
                              const std::u16string& value,
                              int position );

        void setValueFromId( const std::shared_ptr<EObject>& eObject,
                             const std::shared_ptr<EReference>& eReference,
                             const std::u16string& ids );

        void handleUnknownFeature();

    private:
        XmlResource& resource_;
        XmlNamespaces namespaces_;
        bool isPushContext_{false};
        bool isRoot_{false};
        bool isNamespaceAware_{false};
        std::unordered_map<std::u16string, std::shared_ptr<EFactory>> prefixesToFactories_;
        std::stack<std::u16string> elements_;
        std::stack<std::shared_ptr<EObject>> objects_;
    };
} // namespace ecore::impl

#endif