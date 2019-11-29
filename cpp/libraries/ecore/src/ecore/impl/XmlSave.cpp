#include "ecore/impl/XmlSave.hpp"

using namespace ecore;
using namespace ecore::impl;

XmlSave::XmlSave(XmlResource& resource)
    :   resource_(resource)
{
}

XmlSave::~XmlSave()
{
}

void XmlSave::save(std::ostream& o)
{
}

void XmlSave::saveHeader()
{
}

std::shared_ptr<XmlString::Segment> XmlSave::saveTopObject(const std::shared_ptr<EObject>& eObject)
{
    return std::shared_ptr<XmlString::Segment>();
}

void XmlSave::saveNamespaces()
{
}

void XmlSave::saveElementID(const std::shared_ptr<EObject>& eObject)
{
}

bool XmlSave::saveFeatures(const std::shared_ptr<EObject>& eObject, bool attributesOnly)
{
    return false;
}

void XmlSave::saveDataTypeSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveDataTypeMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveManyEmpty(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveEObjectSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveEObjectMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveNil(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveContainedSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveContainedMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveEObjectInternal(const std::shared_ptr<EObjectInternal>& eObjectInternal, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveEObject(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveTypeAttribute(const std::shared_ptr<EClass>& eClass)
{
}

void XmlSave::saveHRefSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveHRefMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveHRef(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveIDRefSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

void XmlSave::saveIDRefMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
}

bool XmlSave::isNil(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
    return false;
}

bool XmlSave::isEmpty(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
    return false;
}

bool XmlSave::shouldSaveFeature(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
    return false;
}

XmlSave::FeatureKind XmlSave::getFeatureKind(const std::shared_ptr<EStructuralFeature>& eFeature)
{
    return FeatureKind();
}

XmlSave::ResourceKind XmlSave::getResourceKind(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
    return ResourceKind();
}

std::string XmlSave::getQName(const std::shared_ptr<EClass>& eClass)
{
    return std::string();
}

std::string XmlSave::getPrefix(const std::shared_ptr<EPackage>& ePackage, bool mustHavePrefix)
{
    return std::string();
}

std::string XmlSave::getDataType(const Any& value, const std::shared_ptr<EStructuralFeature>& eFeature, bool isAttribute)
{
    return std::string();
}

std::string XmlSave::getHRef(const std::shared_ptr<EObject>& eObject)
{
    return std::string();
}

std::string XmlSave::getHRef(const std::shared_ptr<EResource>& eResource, const std::shared_ptr<EObject>& eObject)
{
    return std::string();
}

std::string XmlSave::getIDRef(const std::shared_ptr<EObject>& eObject)
{
    return std::string();
}

std::string XmlSave::getQName(const std::shared_ptr<EPackage>& ePackage, const std::string& name, bool mustHavePrefix)
{
    return std::string();
}
