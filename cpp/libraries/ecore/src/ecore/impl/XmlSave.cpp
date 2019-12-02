#include "ecore/impl/XmlSave.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EList.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EFactory.hpp"

#include <memory>
#include <iterator>
#include <optional>

using namespace ecore;
using namespace ecore::impl;

namespace {
    static constexpr char* XSI_URI = "http://www.w3.org/2001/XMLSchema-instance";
    static constexpr char* XSI_NS = "xsi";
    static constexpr char* XML_NS = "xmlns";
}

XmlSave::XmlSave(XmlResource& resource)
    : resource_(resource)
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
    str_.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    str_.addLine();
}

std::shared_ptr<XmlString::Segment> XmlSave::saveTopObject(const std::shared_ptr<EObject>& eObject)
{
    auto eClass = eObject->eClass();
    auto name = getQName(eClass);
    str_.startElement(name);
    auto mark = str_.mark();
    saveElementID(eObject);
    saveFeatures(eObject, false);
    str_.resetToMark(mark);
    saveNamespaces();
    return mark;
}

void XmlSave::saveNamespaces()
{
    for (auto p : prefixesToURI_)
        str_.addAttribute("xmlns:" + p.first, p.second);
}

void XmlSave::saveElementID(const std::shared_ptr<EObject>& eObject)
{
}

bool XmlSave::saveFeatures(const std::shared_ptr<EObject>& eObject, bool attributesOnly)
{
    auto eClass = eObject->eClass();
    auto eAllFeatures = eClass->getEAllStructuralFeatures();
    std::optional<std::vector<int>> elementFeatures;
    auto elementCount = 0;
    auto i = 0;
    for (auto it = std::begin(*eAllFeatures); it != std::end(*eAllFeatures); ++it, ++i) {
        // current feature
        auto eFeature = *it;

        FeatureKind kind;
        auto itFound = featureKinds_.find(eFeature);
        if (itFound == featureKinds_.end()) {
            kind = featureKinds_[eFeature] = getFeatureKind(eFeature);
        }
        else {
            kind = itFound->second;
        }

        if (kind != TRANSIENT && shouldSaveFeature(eObject, eFeature)) {
            switch (kind) {
            case DATATYPE_SINGLE:
                saveDataTypeSingle(eObject, eFeature);
                continue;
            case DATATYPE_SINGLE_NILLABLE:
                if (!isNil(eObject, eFeature)) {
                    saveDataTypeSingle(eObject, eFeature);
                    continue;
                }
                break;
            case OBJECT_CONTAIN_MANY_UNSETTABLE:
            case DATATYPE_MANY:
                if (isEmpty(eObject, eFeature)) {
                    saveManyEmpty(eObject, eFeature);
                    continue;
                }
            case OBJECT_CONTAIN_SINGLE_UNSETTABLE:
            case OBJECT_CONTAIN_SINGLE:
            case OBJECT_CONTAIN_MANY:
                break;
            case OBJECT_HREF_SINGLE_UNSETTABLE:
                if (isNil(eObject, eFeature))
                    break;
            case OBJECT_HREF_SINGLE:
                switch (getResourceKind(eObject, eFeature)) {
                case CROSS:
                    break;
                case SAME:
                    saveIDRefSingle(eObject, eFeature);
                    continue;
                default:
                    continue;
                }
            case OBJECT_HREF_MANY_UNSETTABLE:
                if (isEmpty(eObject, eFeature)) {
                    saveManyEmpty(eObject, eFeature);
                    continue;
                }
            case OBJECT_HREF_MANY:
                switch (getResourceKind(eObject, eFeature)) {
                case CROSS:
                    break;
                case SAME:
                    saveIDRefMany(eObject, eFeature);
                    continue;
                default:
                    continue;
                }
            default:
                continue;
            }
            if (attributesOnly) {
                continue;
            }
            if (!elementFeatures) {
                elementFeatures = std::vector<int>(eAllFeatures->size());
            }
            elementFeatures->operator[](elementCount++) = i;
        }
    }
    if (!elementFeatures) {
        str_.endEmptyElement();
        return false;
    }
    for (auto i = 0; i < elementCount; i++) {
        auto eFeature = eAllFeatures->get(elementFeatures->operator[](i));
        auto kind = featureKinds_[eFeature];
        switch (kind) {
        case DATATYPE_SINGLE_NILLABLE:
            saveNil(eObject, eFeature);
            break;
        case DATATYPE_MANY:
            saveDataTypeMany(eObject, eFeature);
            break;
        case OBJECT_CONTAIN_SINGLE_UNSETTABLE:
            if (isNil(eObject, eFeature)) {
                saveNil(eObject, eFeature);
                break;
            }
        case OBJECT_CONTAIN_SINGLE:
            saveContainedSingle(eObject, eFeature);
            break;
        case OBJECT_CONTAIN_MANY_UNSETTABLE:
        case OBJECT_CONTAIN_MANY:
            saveContainedMany(eObject, eFeature);
            break;
        case OBJECT_HREF_SINGLE_UNSETTABLE:
            if (isNil(eObject, eFeature)) {
                saveNil(eObject, eFeature);
                break;
            }
        case OBJECT_HREF_SINGLE:
            saveHRefSingle(eObject, eFeature);
            break;
        case OBJECT_HREF_MANY_UNSETTABLE:
        case OBJECT_HREF_MANY:
            saveHRefMany(eObject, eFeature);
            break;
        }
    }

    str_.endElement();
    return true;
}

void XmlSave::saveDataTypeSingle(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
    auto val = eObject->eGet(eFeature);
    auto d = getDataType(val, eFeature, true);
    if (!d.empty())
        str_.addAttribute(getQName(eFeature), d);
}

void XmlSave::saveDataTypeMany(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
    auto val = eObject->eGet(eFeature);
    try {
        auto l = anyCast<std::shared_ptr<EList<Any>>>(val);
        auto d = std::dynamic_pointer_cast<EDataType>(eFeature->getEType());
        auto p = d->getEPackage();
        auto f = p->getEFactoryInstance();
        auto name = getQName(eFeature);
        for (auto value : *l) {
            if (value.empty()) {
                str_.startElement(name);
                str_.addAttribute("xsi:nil", "true");
                str_.endEmptyElement();
                uriToPrefixes_[XSI_URI] = { XSI_NS };
                prefixesToURI_[XSI_NS] = XSI_URI;
            }
            else {
                auto str = f->convertToString(d, value);
                str_.addContent(name, str);
            }
        }
    }
    catch (BadAnyCast & e) {
    }
}

void XmlSave::saveManyEmpty(const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature)
{
    str_.addAttribute(getQName(eFeature), "");
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
