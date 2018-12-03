#include "ecore/ext/EcoreFactoryExt.hpp"

#include <boost/date_time.hpp>

using namespace boost::posix_time;
using namespace ecore;
using namespace ecore::impl;
using namespace ecore::ext;

EcoreFactoryExt::EcoreFactoryExt()
{
}

EcoreFactoryExt::~EcoreFactoryExt()
{
}

boost::any EcoreFactoryExt::createEBooleanFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return literalValue == "true" ? true : false;
}
std::string EcoreFactoryExt::convertEBooleanToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<bool>(instanceValue);
    return value ? "true" : "false";
}
boost::any EcoreFactoryExt::createECharFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return literalValue[0];
}
std::string EcoreFactoryExt::convertECharToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<char>(instanceValue);
    return std::string( 1, value );
}
boost::any EcoreFactoryExt::createEDateFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return from_iso_string( literalValue );
}
std::string EcoreFactoryExt::convertEDateToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<ptime>(instanceValue);
    return to_iso_string( value );
}
boost::any EcoreFactoryExt::createEDoubleFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stod( literalValue );
}
std::string EcoreFactoryExt::convertEDoubleToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<double>(instanceValue);
    return std::to_string( value );
}
boost::any EcoreFactoryExt::createEFloatFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stof( literalValue );
}
std::string EcoreFactoryExt::convertEFloatToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<float>(instanceValue);
    return std::to_string( value );
}
boost::any EcoreFactoryExt::createEIntFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stoi( literalValue );
}
std::string EcoreFactoryExt::convertEIntToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<int>(instanceValue);
    return std::to_string( value );
}
boost::any EcoreFactoryExt::createELongFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stol( literalValue );
}
std::string EcoreFactoryExt::convertELongToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<long>(instanceValue);
    return std::to_string( value );
}
boost::any EcoreFactoryExt::createEShortFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    auto i = std::stoi( literalValue );
    if (i >= std::numeric_limits<short>::min() && i <= std::numeric_limits<short>::max())
        return static_cast<short>(i);
    else
        throw std::out_of_range( "" );
}
std::string EcoreFactoryExt::convertEShortToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<short>(instanceValue);
    return std::to_string( value );
}
boost::any EcoreFactoryExt::createEStringFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return literalValue;
}
std::string EcoreFactoryExt::convertEStringToString( const std::shared_ptr<EDataType>& eDataType, const boost::any& instanceValue ) const
{
    auto value = boost::any_cast<std::string>(instanceValue);
    return value;
}