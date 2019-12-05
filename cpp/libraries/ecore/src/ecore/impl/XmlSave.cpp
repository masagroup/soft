#include "ecore/impl/XmlSave.hpp"
#include "ecore/Any.hpp"
#include "ecore/AnyCast.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EDataType.hpp"
#include "ecore/EFactory.hpp"
#include "ecore/EList.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EPackage.hpp"
#include "ecore/EReference.hpp"
#include "ecore/EStructuralFeature.hpp"
#include "ecore/EcorePackage.hpp"
#include "ecore/impl/EObjectInternal.hpp"
#include "ecore/impl/XmlResource.hpp"

#include <iterator>
#include <memory>
#include <optional>

using namespace ecore;
using namespace ecore::impl;

namespace
{
    static constexpr char* XSI_URI = "http://www.w3.org/2001/XMLSchema-instance";
    static constexpr char* XSI_NS = "xsi";
    static constexpr char* XML_NS = "xmlns";
} // namespace

XmlSave::XmlSave( XmlResource& resource )
    : resource_( resource )
    , keepDefaults_( false )
{
}

XmlSave::~XmlSave()
{
}

void XmlSave::save( std::ostream& o )
{
    auto c = resource_.getContents();
    if( !c || c->empty() )
        return;

    // header
    saveHeader();

    // content
    auto object = c->get( 0 );
    auto mark = saveTopObject( object );

    // namespace
    str_.resetToMark( mark );
    saveNamespaces();

    // write result
    str_.write( o );
}

void XmlSave::saveHeader()
{
    str_.add( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
    str_.addLine();
}

std::shared_ptr<XmlString::Segment> XmlSave::saveTopObject( const std::shared_ptr<EObject>& eObject )
{
    auto eClass = eObject->eClass();
    auto name = getQName( eClass );
    str_.startElement( name );
    auto mark = str_.mark();
    saveElementID( eObject );
    saveFeatures( eObject, false );
    return mark;
}

void XmlSave::saveNamespaces()
{
    for( auto p : prefixesToURI_ )
        str_.addAttribute( "xmlns:" + p.first, p.second );
}

void XmlSave::saveElementID( const std::shared_ptr<EObject>& eObject )
{
}

bool XmlSave::saveFeatures( const std::shared_ptr<EObject>& eObject, bool attributesOnly )
{
    auto eClass = eObject->eClass();
    auto eAllFeatures = eClass->getEAllStructuralFeatures();
    std::optional<std::vector<int>> elementFeatures;
    auto elementCount = 0;
    auto i = 0;
    for( auto it = std::begin( *eAllFeatures ); it != std::end( *eAllFeatures ); ++it, ++i )
    {
        // current feature
        auto eFeature = *it;

        FeatureKind kind;
        auto itFound = featureKinds_.find( eFeature );
        if( itFound == featureKinds_.end() )
        {
            kind = featureKinds_[eFeature] = getFeatureKind( eFeature );
        }
        else
        {
            kind = itFound->second;
        }

        if( kind != TRANSIENT && shouldSaveFeature( eObject, eFeature ) )
        {
            switch( kind )
            {
            case DATATYPE_SINGLE:
                saveDataTypeSingle( eObject, eFeature );
                continue;
            case DATATYPE_SINGLE_NILLABLE:
                if( !isNil( eObject, eFeature ) )
                {
                    saveDataTypeSingle( eObject, eFeature );
                    continue;
                }
                break;
            case OBJECT_CONTAIN_MANY_UNSETTABLE:
            case DATATYPE_MANY:
                if( isEmpty( eObject, eFeature ) )
                {
                    saveManyEmpty( eObject, eFeature );
                    continue;
                }
                break;
            case OBJECT_CONTAIN_SINGLE_UNSETTABLE:
            case OBJECT_CONTAIN_SINGLE:
            case OBJECT_CONTAIN_MANY:
                break;
            case OBJECT_HREF_SINGLE_UNSETTABLE:
                if( isNil( eObject, eFeature ) )
                    break;
            case OBJECT_HREF_SINGLE:
                switch( getResourceKind( eObject, eFeature ) )
                {
                case CROSS:
                    break;
                case SAME:
                    saveIDRefSingle( eObject, eFeature );
                    continue;
                default:
                    continue;
                }
                break;
            case OBJECT_HREF_MANY_UNSETTABLE:
                if( isEmpty( eObject, eFeature ) )
                {
                    saveManyEmpty( eObject, eFeature );
                    continue;
                }
            case OBJECT_HREF_MANY:
                switch( getResourceKind( eObject, eFeature ) )
                {
                case CROSS:
                    break;
                case SAME:
                    saveIDRefMany( eObject, eFeature );
                    continue;
                default:
                    continue;
                }
                break;
            default:
                continue;
            }
            if( attributesOnly )
            {
                continue;
            }
            if( !elementFeatures )
            {
                elementFeatures = std::vector<int>( eAllFeatures->size() );
            }
            elementFeatures->operator[]( elementCount++ ) = i;
        }
    }
    if( !elementFeatures )
    {
        str_.endEmptyElement();
        return false;
    }
    for( auto i = 0; i < elementCount; i++ )
    {
        auto eFeature = eAllFeatures->get( elementFeatures->operator[]( i ) );
        auto kind = featureKinds_[eFeature];
        switch( kind )
        {
        case DATATYPE_SINGLE_NILLABLE:
            saveNil( eObject, eFeature );
            break;
        case DATATYPE_MANY:
            saveDataTypeMany( eObject, eFeature );
            break;
        case OBJECT_CONTAIN_SINGLE_UNSETTABLE:
            if( isNil( eObject, eFeature ) )
            {
                saveNil( eObject, eFeature );
                break;
            }
        case OBJECT_CONTAIN_SINGLE:
            saveContainedSingle( eObject, eFeature );
            break;
        case OBJECT_CONTAIN_MANY_UNSETTABLE:
        case OBJECT_CONTAIN_MANY:
            saveContainedMany( eObject, eFeature );
            break;
        case OBJECT_HREF_SINGLE_UNSETTABLE:
            if( isNil( eObject, eFeature ) )
            {
                saveNil( eObject, eFeature );
                break;
            }
        case OBJECT_HREF_SINGLE:
            saveHRefSingle( eObject, eFeature );
            break;
        case OBJECT_HREF_MANY_UNSETTABLE:
        case OBJECT_HREF_MANY:
            saveHRefMany( eObject, eFeature );
            break;
        }
    }

    str_.endElement();
    return true;
}

void XmlSave::saveDataTypeSingle( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto d = getDataType( val, eFeature, true );
    if( !d.empty() )
        str_.addAttribute( getQName( eFeature ), d );
}

void XmlSave::saveDataTypeMany( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto l = anyCast<std::shared_ptr<EList<Any>>>( val );
    auto d = std::dynamic_pointer_cast<EDataType>( eFeature->getEType() );
    auto p = d->getEPackage();
    auto f = p->getEFactoryInstance();
    auto name = getQName( eFeature );
    for( auto value : *l )
    {
        if( value.empty() )
        {
            str_.startElement( name );
            str_.addAttribute( "xsi:nil", "true" );
            str_.endEmptyElement();
            uriToPrefixes_[XSI_URI] = {XSI_NS};
            prefixesToURI_[XSI_NS] = XSI_URI;
        }
        else
        {
            auto str = f->convertToString( d, value );
            str_.addContent( name, str );
        }
    }
}

void XmlSave::saveManyEmpty( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    str_.addAttribute( getQName( eFeature ), "" );
}

void XmlSave::saveEObjectSingle( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto obj = anyObjectCast<std::shared_ptr<EObject>>( val );
    if( obj )
    {
        auto id = getHRef( obj );
        str_.addAttribute( getQName( eFeature ), "" );
    }
}

void XmlSave::saveEObjectMany( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto l = anyListCast<std::shared_ptr<EObject>>( val );
    auto failure = false;
    std::string s = "";
    for( auto it = std::begin( *l ); it != std::end( *l ); ++it )
    {
        auto obj = *it;
        auto id = getHRef( obj );
        if( id.empty() )
        {
            failure = true;
            break;
        }
        else
        {
            if( it != std::begin( *l ) )
                s += " ";
            s += id;
        }
    }
    if( !failure && !s.empty() )
        str_.addAttribute( getQName( eFeature ), s );
}

void XmlSave::saveNil( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    str_.addNil( getQName( eFeature ) );
}

void XmlSave::saveContainedSingle( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto obj = anyObjectCast<std::shared_ptr<EObject>>( val );
    auto internal = std::dynamic_pointer_cast<EObjectInternal>( obj );
    if( internal )
        saveEObjectInternal( internal, eFeature );
}

void XmlSave::saveContainedMany( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto l = anyListCast<std::shared_ptr<EObject>>( val );
    for( auto obj : *l )
    {
        auto internal = std::dynamic_pointer_cast<EObjectInternal>( obj );
        saveEObjectInternal( internal, eFeature );
    }
}

void XmlSave::saveEObjectInternal( const std::shared_ptr<EObjectInternal>& eObjectInternal,
                                   const std::shared_ptr<EStructuralFeature>& eFeature )
{
    if( eObjectInternal->eDirectResource() || eObjectInternal->eIsProxy() )
        saveHRef( eObjectInternal, eFeature );
    else
        saveEObject( eObjectInternal, eFeature );
}

void XmlSave::saveEObject( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    str_.startElement( getQName( eFeature ) );
    auto eClass = eObject->eClass();
    auto eType = eFeature->getEType();
    if( eType != eClass && eType != EcorePackage::eInstance()->getEObject() )
    {
        saveTypeAttribute( eClass );
    }
    saveElementID( eObject );
    saveFeatures( eObject, false );
}

void XmlSave::saveTypeAttribute( const std::shared_ptr<EClass>& eClass )
{
    str_.addAttribute( "xsi:type", getQName( eClass ) );
    uriToPrefixes_[XSI_URI] = {XSI_NS};
    prefixesToURI_[XSI_NS] = XSI_URI;
}

void XmlSave::saveHRefSingle( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto value = eObject->eGet( eFeature );
    auto o = anyObjectCast<std::shared_ptr<EObject>>( value );
    if( o )
        saveHRef( o, eFeature );
}

void XmlSave::saveHRefMany( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto l = anyListCast<std::shared_ptr<EObject>>( val );
    for( auto obj : *l )
        saveHRef( obj, eFeature );
}

void XmlSave::saveHRef( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto href = getHRef( eObject );
    if( href.empty() )
    {
        str_.startElement( getQName( eFeature ) );
        auto eClass = eObject->eClass();
        auto eType = std::dynamic_pointer_cast<EClass>( eFeature->getEType() );
        if( eType != eClass && eType && eType->isAbstract() )
            saveTypeAttribute( eClass );
        str_.addAttribute( "href", href );
        str_.endEmptyElement();
    }
}

void XmlSave::saveIDRefSingle( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto value = eObject->eGet( eFeature );
    auto obj = anyObjectCast<std::shared_ptr<EObject>>( value );
    if( obj )
    {
        auto id = getIDRef( obj );
        if( !id.empty() )
            str_.addAttribute( getQName( eFeature ), id );
    }
}

void XmlSave::saveIDRefMany( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto l = anyListCast<std::shared_ptr<EObject>>( val );
    auto failure = false;
    std::string s = "";
    for( auto it = std::begin( *l ); it != std::end( *l ); ++it )
    {
        auto o = *it;
        auto id = getIDRef( o );
        if( id.empty() )
        {
            failure = true;
            break;
        }
        else
        {
            if( it != std::begin( *l ) )
                s += " ";
            s += id;
        }
    }
    if( !failure && !s.empty() )
        str_.addAttribute( getQName( eFeature ), s );
}

bool XmlSave::isNil( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    return eObject->eGet( eFeature ).empty();
}

bool XmlSave::isEmpty( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto value = eObject->eGet( eFeature );
    auto l = anyCast<std::shared_ptr<EList<Any>>>( value );
    return l->empty();
}

bool XmlSave::shouldSaveFeature( const std::shared_ptr<EObject>& eObject, const std::shared_ptr<EStructuralFeature>& eFeature )
{
    return eObject->eIsSet( eFeature ) || (keepDefaults_ && !eFeature->getDefaultValueLiteral().empty());
}

XmlSave::FeatureKind XmlSave::getFeatureKind( const std::shared_ptr<EStructuralFeature>& eFeature )
{
    if( eFeature->isTransient() )
        return TRANSIENT;

    auto isMany = eFeature->isMany();
    auto isUnsettable = eFeature->isUnsettable();

    auto eReference = std::dynamic_pointer_cast<EReference>( eFeature );
    if( eReference )
    {
        if( eReference->isContainment() )
            return isMany ? isUnsettable ? OBJECT_CONTAIN_MANY_UNSETTABLE : OBJECT_CONTAIN_MANY
                          : isUnsettable ? OBJECT_CONTAIN_SINGLE_UNSETTABLE : OBJECT_CONTAIN_SINGLE;
        auto opposite = eReference->getEOpposite();
        if( opposite && opposite->isContainment() )
            return TRANSIENT;
        return isMany ? isUnsettable ? OBJECT_HREF_MANY_UNSETTABLE : OBJECT_HREF_MANY
                      : isUnsettable ? OBJECT_HREF_SINGLE_UNSETTABLE : OBJECT_HREF_SINGLE;
    }
    else
    {
        // Attribute
        auto d = std::dynamic_pointer_cast<EDataType>( eFeature->getEType() );
        if( !d->isSerializable() )
            return TRANSIENT;
        if( isMany )
            return DATATYPE_MANY;

        if( isUnsettable )
            return DATATYPE_SINGLE_NILLABLE;
        else
            return DATATYPE_SINGLE;
    }
}

XmlSave::ResourceKind XmlSave::getResourceKind( const std::shared_ptr<EObject>& eObject,
                                                const std::shared_ptr<EStructuralFeature>& eFeature )
{
    auto val = eObject->eGet( eFeature );
    auto internal = anyObjectCast<std::shared_ptr<EObject>>( val );
    if( !internal )
        return SKIP;
    else if( internal->eIsProxy() )
        return CROSS;
    else
    {
        auto resource = internal->eResource();
        if( resource == resource_.getThisPtr() || !resource )
            return SAME;
        else
            return CROSS;
    }
}

std::string XmlSave::getQName( const std::shared_ptr<EClass>& eClass )
{
    return getQName( eClass->getEPackage(), eClass->getName(), false );
}

std::string XmlSave::getQName( const std::shared_ptr<EStructuralFeature>& eFeature )
{
    return eFeature->getName();
}

std::string XmlSave::getQName( const std::shared_ptr<EPackage>& ePackage, const std::string& name, bool mustHavePrefix )
{
    auto nsPrefix = getPrefix( ePackage, mustHavePrefix );
    if( nsPrefix.empty() )
        return name;
    else if( name.empty() )
        return nsPrefix;
    else
        return nsPrefix + ":" + name;
}

std::string XmlSave::getPrefix( const std::shared_ptr<EPackage>& ePackage, bool mustHavePrefix )
{
    std::string nsPrefix;
    auto itFound = packages_.find( ePackage );
    if( itFound != packages_.end() )
        nsPrefix = itFound->second;
    else
    {
        auto nsURI = ePackage->getNsURI();
        auto found = false;
        auto& prefixes = uriToPrefixes_[nsURI];
        for( auto prefix : prefixes )
        {
            nsPrefix = prefix;
            if( !mustHavePrefix || !nsPrefix.empty() )
            {
                found = true;
                break;
            }
        }
        if( !found )
        {
            nsPrefix = namespaces_.getPrefix( nsPrefix );
            if( !nsPrefix.empty() )
                return nsPrefix;

            nsPrefix = ePackage->getNsPrefix();
            if( nsPrefix.empty() && mustHavePrefix )
                nsPrefix = "_";

            auto itPref = prefixesToURI_.find( nsPrefix );
            if( itPref != prefixesToURI_.end() && itPref->second != nsURI )
            {
                auto index = 1;
                while( prefixesToURI_.find( nsPrefix + "_" + std::to_string( index ) ) != prefixesToURI_.end() )
                    index++;
                nsPrefix += "_" + std::to_string( index );
            }
            prefixesToURI_[nsPrefix] = nsURI;
        }
        packages_[ePackage] = nsPrefix;
    }
    return nsPrefix;
}

std::string XmlSave::getDataType( const Any& value, const std::shared_ptr<EStructuralFeature>& eFeature, bool isAttribute )
{
    if( value.empty() )
        return "";
    else
    {
        auto d = std::dynamic_pointer_cast<EDataType>( eFeature->getEType() );
        auto p = d->getEPackage();
        auto f = p->getEFactoryInstance();
        auto s = f->convertToString( d, value );
        return s;
    }
}

std::string XmlSave::getHRef( const std::shared_ptr<EObject>& eObject )
{
    auto internal = std::dynamic_pointer_cast<EObjectInternal>( eObject );
    if( internal )
    {
        auto uri = internal->eProxyURI();
        if( uri.isEmpty() )
        {
            auto eResource = eObject->eResource();
            if( eResource )
                return getHRef( eResource, eObject );
            else
                return std::string();
        }
        else
        {
            return uri.toString();
        }
    }
    return std::string();
}

std::string XmlSave::getHRef( const std::shared_ptr<EResource>& eResource, const std::shared_ptr<EObject>& eObject )
{
    auto uri = eResource->getURI();
    auto fragment = eResource->getURIFragment( eObject );
    uri.setFragment( fragment );
    return uri.toString();
}

std::string XmlSave::getIDRef( const std::shared_ptr<EObject>& eObject )
{
    return resource_.getURIFragment( eObject );
}
