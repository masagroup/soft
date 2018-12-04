// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECOREFACTORY_ECOREFACTORYEXT_HPP
#define ECORE_ECOREFACTORY_ECOREFACTORYEXT_HPP

#include "ecore/impl/EcoreFactoryImpl.hpp"

namespace ecore::ext
{

    class EcoreFactoryExt : public virtual impl::EcoreFactoryImpl
    {
    private:
        EcoreFactoryExt( EcoreFactoryExt const& ) = delete;
        EcoreFactoryExt& operator=( EcoreFactoryExt const& ) = delete;

    protected:
        friend class EcoreFactory;
        EcoreFactoryExt();

    public:
        virtual ~EcoreFactoryExt();

    protected:
        virtual boost::any createEBooleanFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createECharFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createEDateFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createEDoubleFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createEFloatFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createEIntFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createELongFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createEShortFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;
        virtual boost::any createEStringFromString( const std::shared_ptr<ecore::EDataType>& dataType, const std::string& literalValue ) const;

        virtual std::string convertEBooleanToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertECharToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertEDateToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertEDoubleToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertEFloatToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertEIntToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertELongToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertEShortToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;
        virtual std::string convertEStringToString( const std::shared_ptr<ecore::EDataType>& dataType, const boost::any& literalValue ) const;

    private:
        static EcoreFactoryExt* create();
    };

}

#endif