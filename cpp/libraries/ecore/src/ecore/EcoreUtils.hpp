// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECORE_UTILS_HPP_
#define ECORE_ECORE_UTILS_HPP_

#include "ecore/Exports.hpp"
#include "ecore/Uri.hpp"

#include <string>
#include <memory>

namespace ecore
{
    class Any;
    class EObject;
    class EDataType;
    class EResource;
    class EResourceSet;


    class ECORE_API EcoreUtils
    {
    public:
        static std::string getID( const std::shared_ptr<EObject>& eObject );

        static void setID( const std::shared_ptr<EObject>& eObject, const std::string& id );

        static std::string convertToString( const std::shared_ptr<EDataType>& eDataType, const Any& value );

        static Any createFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literal );

        static URI getURI(const std::shared_ptr<EObject>& eObject);

        static std::shared_ptr<EObject> getEObject(const std::shared_ptr<EObject>& rootEObject, const std::string& relativeFragmentPath);
        
        static std::shared_ptr<EObject> resolve(const std::shared_ptr<EObject>& proxy, const std::shared_ptr<EObject>& context );

        static std::shared_ptr<EObject> resolve( const std::shared_ptr<EObject>& proxy, const std::shared_ptr<EResource>& context );

        static std::shared_ptr<EObject> resolve( const std::shared_ptr<EObject>& proxy, const std::shared_ptr<EResourceSet>& context );


    private:

        static std::string getRelativeURIFragmentPath(const std::shared_ptr<EObject>& ancestor, const std::shared_ptr<EObject>& descendant, bool resolve);
    };

} // namespace ecore::impl

#endif