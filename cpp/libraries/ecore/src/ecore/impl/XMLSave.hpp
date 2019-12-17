// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_XMLSAVE_HPP_
#define ECORE_XMLSAVE_HPP_

#include "ecore/Any.hpp"
#include "ecore/impl/XMLNamespaces.hpp"
#include "ecore/impl/XMLString.hpp"

#include <map>

namespace ecore {
    class EClass;
    class EObject;
    class EPackage;
    class EResource;
    class EStructuralFeature;
}

namespace ecore::impl
{
    class EObjectInternal;

    class XMLResource;

    class XMLSave
    {
    public:
        XMLSave(XMLResource& resource);

        virtual ~XMLSave();

        void save(std::ostream& o);

    protected:
        void saveHeader();
        std::shared_ptr<XMLString::Segment> saveTopObject(const std::shared_ptr<EObject>& eObject);
        virtual void saveNamespaces();
        void saveElementID(const std::shared_ptr<EObject>& eObject);
        bool saveFeatures(const std::shared_ptr<EObject>& eObject, bool attributesOnly);

        void saveDataTypeSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        void saveDataTypeMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        void saveManyEmpty(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        void saveEObjectSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        void saveEObjectMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        void saveNil(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        void saveContainedSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        void saveContainedMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        void saveEObjectInternal(const std::shared_ptr<EObjectInternal>& eObjectInternal, const std::shared_ptr<EStructuralFeature>& eFeature);
        void saveEObject(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        void saveTypeAttribute(const std::shared_ptr<EClass>& eClass);

        void saveHRefSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        void saveHRefMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        void saveHRef(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        void saveIDRefSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        void saveIDRefMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        bool isNil(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        bool isEmpty(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        bool shouldSaveFeature(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);

        enum FeatureKind {
            TRANSIENT,
            DATATYPE_SINGLE,
            DATATYPE_ELEMENT_SINGLE,
            DATATYPE_CONTENT_SINGLE,
            DATATYPE_SINGLE_NILLABLE,
            DATATYPE_MANY,
            OBJECT_CONTAIN_SINGLE,
            OBJECT_CONTAIN_MANY,
            OBJECT_HREF_SINGLE,
            OBJECT_HREF_MANY,
            OBJECT_CONTAIN_SINGLE_UNSETTABLE,
            OBJECT_CONTAIN_MANY_UNSETTABLE,
            OBJECT_HREF_SINGLE_UNSETTABLE,
            OBJECT_HREF_MANY_UNSETTABLE,
            OBJECT_ELEMENT_SINGLE,
            OBJECT_ELEMENT_SINGLE_UNSETTABLE,
            OBJECT_ELEMENT_MANY,
            OBJECT_ELEMENT_IDREF_SINGLE,
            OBJECT_ELEMENT_IDREF_SINGLE_UNSETTABLE,
            OBJECT_ELEMENT_IDREF_MANY,
            ATTRIBUTE_FEATURE_MAP,
            ELEMENT_FEATURE_MAP,
            OBJECT_ATTRIBUTE_SINGLE,
            OBJECT_ATTRIBUTE_MANY,
            OBJECT_ATTRIBUTE_IDREF_SINGLE,
            OBJECT_ATTRIBUTE_IDREF_MANY,
            DATATYPE_ATTRIBUTE_MANY,
        };
        
        FeatureKind getFeatureKind(const std::shared_ptr<EStructuralFeature>& eFeature);

        enum ResourceKind {
            SKIP,
            SAME,
            CROSS,
        };

        ResourceKind getResourceKindSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature);
        ResourceKind getResourceKindMany( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature );


        std::string getQName(const std::shared_ptr<EClass>& eClass);
        std::string getQName(const std::shared_ptr<EPackage>& ePackage , const std::string& name, bool mustHavePrefix);
        std::string getQName(const std::shared_ptr<EStructuralFeature>& eFeature);
        std::string getPrefix(const std::shared_ptr<EPackage>& ePackage, bool mustHavePrefix);
        std::string getDataType(const Any& value, const std::shared_ptr<EStructuralFeature>& eFeature, bool isAttribute);
        std::string getHRef(const std::shared_ptr<EObject>& eObject);
        std::string getHRef(const std::shared_ptr<EResource>& eResource, const std::shared_ptr<EObject>& eObject);
        std::string getIDRef(const std::shared_ptr<EObject>& eObject);

    protected:
        XMLResource& resource_;
        XMLNamespaces namespaces_;
        XMLString str_;
        std::map<std::shared_ptr<EPackage>, std::string> packages_;
        std::map<std::string, std::vector<std::string>> uriToPrefixes_;
        std::map<std::string, std::string> prefixesToURI_;
        std::map<std::shared_ptr< EStructuralFeature>, FeatureKind> featureKinds_;
        bool keepDefaults_;
    };
}

#endif