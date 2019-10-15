#include "ecore/EcoreUtils.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EResource.hpp"
#include "ecore/impl/EObjectInternal.hpp"
#include "ecore/impl/StringUtils.hpp"

#include <deque>
#include <unordered_set>

using namespace ecore;
using namespace ecore::impl;

std::string EcoreUtils::getID(const std::shared_ptr<EObject>& eObject)
{
    std::shared_ptr<EClass> eClass = eObject->eClass();
    std::shared_ptr<EAttribute> eIDAttribute = eClass->getEIDAttribute();
    return !eIDAttribute || !eObject->eIsSet(eIDAttribute)
        ? std::string()
        : convertToString(eIDAttribute->getEAttributeType(), eObject->eGet(eIDAttribute));

    return std::string();
}

void EcoreUtils::setID(const std::shared_ptr<EObject>& eObject, const std::string& id)
{
    std::shared_ptr<EClass> eClass = eObject->eClass();
    std::shared_ptr<EAttribute> eIDAttribute = eClass->getEIDAttribute();
    if (!eIDAttribute)
        throw std::runtime_error("The object doesn't have an ID feature.");
    else if (id.empty())
        eObject->eUnset(eIDAttribute);
    else
        eObject->eSet(eIDAttribute, createFromString(eIDAttribute->getEAttributeType(), id));
}

std::string EcoreUtils::convertToString(const std::shared_ptr<EDataType>& eDataType, const Any& value)
{
    auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
    return eFactory->convertToString(eDataType, value);
}

Any EcoreUtils::createFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literal)
{
    auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
    return eFactory->createFromString(eDataType, literal);
}

URI EcoreUtils::getURI(const std::shared_ptr<EObject>& eObject)
{
    if (eObject->eIsProxy())
        return eObject->eProxyURI();
    else
    {
        std::shared_ptr<EResource> resource = eObject->eResource();
        if (resource) {
            auto uri = resource->getURI();
            auto uriFragment = resource->getURIFragment(eObject);
            return URI( uri.toString() + "#" + uriFragment);
        }
        else {
            std::string id = getID(eObject);
            if (id.empty())
                return URI("#//" + getRelativeURIFragmentPath(nullptr, eObject, false));
            else
                return URI("#" + id);
        }
    }
    return URI();
}

std::string EcoreUtils::getRelativeURIFragmentPath(const std::shared_ptr<EObject>& ancestor, const std::shared_ptr<EObject>& descendant, bool resolve)
{
    if (ancestor == descendant)
        return "";
   
    std::unordered_set<std::shared_ptr<EObject>> visited;
    std::deque<std::string> fragmentURIPath;
    auto internalEObject = descendant;
    for (auto eContainer = internalEObject->eContainer();
        eContainer && visited.insert(eContainer).second;
        eContainer = internalEObject->eContainer())
    {
        auto internalEContainer = std::dynamic_pointer_cast<EObjectInternal>(eContainer);
        fragmentURIPath.push_front(internalEContainer->eURIFragmentSegment(internalEObject->eContainingFeature(), internalEObject));
        internalEObject = internalEContainer;
        if (internalEContainer == ancestor)
        {
            break;
        }
    }

    if (internalEObject != ancestor && ancestor)
        throw "The ancestor not found'";

    return join(fragmentURIPath, "/");
}

std::shared_ptr<EObject> getEObject(const std::shared_ptr<EObject>& rootEObject, const std::string& relativeFragmentPath) {
    auto segments = split(relativeFragmentPath, "/");
    auto eObject = std::dynamic_pointer_cast<EObjectInternal>(rootEObject);
    for (int i = 0; i < segments.size() && eObject; ++i)
        eObject = std::dynamic_pointer_cast<EObjectInternal>(eObject->eObjectForFragmentSegment(std::string(segments[i])));
    return eObject;

}
