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

#include "ecore/Any.hpp"
#include "ecore/impl/XmlNamespaces.hpp"

#include <xercesc/sax/Locator.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include <stack>
#include <string>
#include <unordered_map>

namespace ecore
{
    class EClassifier;
    class EDiagnostic;
    class EFactory;
    class EObject;
    class EPackageRegistry;
    class EReference;
    class EStructuralFeature;

} // namespace ecore

namespace ecore::impl
{
    class XmlResource;

    class XmlLoad : public xercesc::DefaultHandler
    {
    public:
        XmlLoad( XmlResource& resource );

        virtual ~XmlLoad();

        virtual void setDocumentLocator( const xercesc::Locator* const locator );

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
        

    private:
        const xercesc::Attributes* setAttributes( const xercesc::Attributes* attrs );
        void startElement( const std::string uri, const std::string& localName, const std::string& qname );
        void processElement( const std::string& name, const std::string& prefix, const std::string& localName );
        
        void handleSchemaLocation();
        void handleXSISchemaLocation( const std::string& schemaLocation );
        void handleXSINoNamespaceSchemaLocation( const std::string& schemaLocation );
        void handleAttributes( const std::shared_ptr<EObject>& eObject );
        void handleProxy( const std::shared_ptr<EObject>& eObject , const std::string& id);
        
        std::shared_ptr<EFactory> getFactoryForPrefix( const std::string& prefix );
        std::shared_ptr<EStructuralFeature> getFeature( const std::shared_ptr<EObject>& eObject, const std::string& name );

        std::shared_ptr<EObject> createObject( const std::string& prefix, const std::string& localName );
        std::shared_ptr<EObject> createObject( const std::shared_ptr<EFactory>& eFactory, const std::shared_ptr<EClassifier>& type );
        std::shared_ptr<EObject> createObjectFromFeatureType( const std::shared_ptr<EObject>& eObject,
                                                              const std::shared_ptr<EStructuralFeature>& feature );
        std::shared_ptr<EObject> createObjectFromTypeName( const std::shared_ptr<EObject>& eObject,
                                                           const std::string& typeQName,
                                                           const std::shared_ptr<EStructuralFeature>& feature );
        
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
                              const Any& value,
                              int position = -1 );

        void setAttributeValue( const std::shared_ptr<EObject>& eObject, const std::string& name, const std::string& value );

        void setValueFromId( const std::shared_ptr<EObject>& eObject,
                             const std::shared_ptr<EReference>& eReference,
                             const std::string& ids );

        std::string getLocation() const;
        int getLineNumber() const;
        int getColumnNumber() const;

        void handleFeature( const std::string& prefix, const std::string& localName );
        void handleUnknownFeature( const std::string& name );
        void handleUnknownPackage( const std::string& name );

        void handleReferences();

        void error( const std::shared_ptr<EDiagnostic>& diagnostic );
        void warning( const std::shared_ptr<EDiagnostic>& diagnostic );

    private:
        struct Reference;

        XmlResource& resource_;
        XmlNamespaces namespaces_;
        const xercesc::Locator* locator_{nullptr};
        const xercesc::Attributes* attributes_{nullptr};
        bool isResolveDeferred_{false};
        bool isPushContext_{false};
        bool isRoot_{false};
        bool isNamespaceAware_{false};
        std::shared_ptr<EPackageRegistry> packageRegistry_;
        std::unordered_map<std::string, std::shared_ptr<EFactory>> prefixesToFactories_;
        std::stack<std::string> elements_;
        std::stack<std::shared_ptr<EObject>> objects_;
        std::vector<std::shared_ptr<EObject>> sameDocumentProxies_;
        std::vector<Reference> references_;
    };
} // namespace ecore::impl

#endif