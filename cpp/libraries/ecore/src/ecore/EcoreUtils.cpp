#include "ecore/EcoreUtils.hpp"
#include "ecore/EAttribute.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EPackageRegistry.hpp"
#include "ecore/EResource.hpp"
#include "ecore/EResourceSet.hpp"
#include "ecore/impl/EObjectInternal.hpp"
#include "ecore/impl/StringUtils.hpp"

#include <deque>
#include <unordered_set>

using namespace ecore;
using namespace ecore::impl;

std::string EcoreUtils::getID( const std::shared_ptr<EObject>& eObject )
{
    std::shared_ptr<EClass> eClass = eObject->eClass();
    std::shared_ptr<EAttribute> eIDAttribute = eClass->getEIDAttribute();
    return !eIDAttribute || !eObject->eIsSet( eIDAttribute )
               ? std::string()
               : convertToString( eIDAttribute->getEAttributeType(), eObject->eGet( eIDAttribute ) );

    return std::string();
}

void EcoreUtils::setID( const std::shared_ptr<EObject>& eObject, const std::string& id )
{
    std::shared_ptr<EClass> eClass = eObject->eClass();
    std::shared_ptr<EAttribute> eIDAttribute = eClass->getEIDAttribute();
    if( !eIDAttribute )
        throw std::runtime_error( "The object doesn't have an ID feature." );
    else if( id.empty() )
        eObject->eUnset( eIDAttribute );
    else
        eObject->eSet( eIDAttribute, createFromString( eIDAttribute->getEAttributeType(), id ) );
}

std::string EcoreUtils::convertToString( const std::shared_ptr<EDataType>& eDataType, const Any& value )
{
    auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
    return eFactory->convertToString( eDataType, value );
}

Any EcoreUtils::createFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literal )
{
    auto eFactory = eDataType->getEPackage()->getEFactoryInstance();
    return eFactory->createFromString( eDataType, literal );
}

URI EcoreUtils::getURI( const std::shared_ptr<EObject>& eObject )
{
    if( eObject->eIsProxy() )
        return eObject->getInternal().eProxyURI();
    else
    {
        std::shared_ptr<EResource> resource = eObject->eResource();
        if( resource )
        {
            auto uri = resource->getURI();
            auto uriFragment = resource->getURIFragment( eObject );
            return URI( uri.toString() + "#" + uriFragment );
        }
        else
        {
            std::string id = getID( eObject );
            if( id.empty() )
                return URI( "#//" + getRelativeURIFragmentPath( nullptr, eObject, false ) );
            else
                return URI( "#" + id );
        }
    }
    return URI();
}

std::string EcoreUtils::getRelativeURIFragmentPath( const std::shared_ptr<EObject>& ancestor,
                                                    const std::shared_ptr<EObject>& descendant,
                                                    bool resolve )
{
    if( ancestor == descendant )
        return "";

    std::unordered_set<std::shared_ptr<EObject>> visited;
    std::deque<std::string> fragmentURIPath;
    auto eObject = descendant;
    for( auto eContainer = eObject->eContainer(); eContainer && visited.insert( eContainer ).second;
         eContainer = eObject->eContainer() )
    {
        fragmentURIPath.push_front(eContainer->getInternal().eURIFragmentSegment(eObject->eContainingFeature(), eObject ) );
        eObject = eContainer;
        if(eContainer == ancestor )
        {
            break;
        }
    }

    if(eObject != ancestor && ancestor )
        throw "The ancestor not found'";

    return join( fragmentURIPath, "/" );
}

std::shared_ptr<EObject> EcoreUtils::getEObject( const std::shared_ptr<EObject>& rootEObject, const std::string& relativeFragmentPath )
{
    auto segments = split( relativeFragmentPath, "/" );
    auto eObject = rootEObject;
    for (int i = 0; i < segments.size() && eObject; ++i)
        eObject = eObject->getInternal().eObjectForFragmentSegment(std::string(segments[i]));
    return eObject;
}

std::shared_ptr<EObject> EcoreUtils::resolve( const std::shared_ptr<EObject>& proxy, const std::shared_ptr<EObject>& context )
{
    auto resource = context ? context->eResource() : nullptr;
    return resolve( proxy, resource ? resource->getResourceSet() : nullptr );
}

std::shared_ptr<EObject> EcoreUtils::resolve( const std::shared_ptr<EObject>& proxy, const std::shared_ptr<EResource>& resource )
{
    return resolve( proxy, resource ? resource->getResourceSet() : nullptr );
}

std::shared_ptr<EObject> EcoreUtils::resolve( const std::shared_ptr<EObject>& proxy, const std::shared_ptr<EResourceSet>& resourceSet )
{
    URI proxyURI = proxy->getInternal().eProxyURI();
    if( !proxyURI.isEmpty() )
    {
        std::shared_ptr<EObject> resolved;
        if( resourceSet )
        {
            try
            {
                resolved = resourceSet->getEObject( proxyURI, true );
            }
            catch( ... )
            {
                resolved = resourceSet->getEObject( proxyURI, false );
            }
        }
        else
        {
            auto ePackage = EPackageRegistry::getInstance()->getPackage( proxyURI.trimFragment().toString() );
            if( ePackage )
            {
                auto eResource = ePackage->eResource();
                if( eResource )
                    resolved = eResource->getEObject( proxyURI.getFragment() );
            }
        }
        if(resolved && resolved != proxy)
            return resolve(resolved,resourceSet);
    }
    return proxy;

}
