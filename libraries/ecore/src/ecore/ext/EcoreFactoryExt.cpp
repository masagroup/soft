#include "ecore/ext/EcoreFactoryExt.hpp"
#include <ctime>
#include <sstream>

using namespace ecore;
using namespace ecore::impl;
using namespace ecore::ext;

EcoreFactoryExt::EcoreFactoryExt()
{
}

EcoreFactoryExt::~EcoreFactoryExt()
{
}

Any EcoreFactoryExt::createEBooleanFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return literalValue == "true" ? true : false;
}
std::string EcoreFactoryExt::convertEBooleanToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<bool>(instanceValue);
    return value ? "true" : "false";
}
Any EcoreFactoryExt::createECharFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return literalValue[0];
}
std::string EcoreFactoryExt::convertECharToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<char>(instanceValue);
    return std::string( 1, value );
}
Any EcoreFactoryExt::createEDateFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    std::istringstream stream( literalValue );
    std::time_t t;
    stream >> t;
    return t;
}
std::string EcoreFactoryExt::convertEDateToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<std::time_t>(instanceValue);
    std::ostringstream stream;
    stream << value;
    return stream.str();
}
Any EcoreFactoryExt::createEDoubleFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stod( literalValue );
}
std::string EcoreFactoryExt::convertEDoubleToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<double>(instanceValue);
    return std::to_string( value );
}
Any EcoreFactoryExt::createEFloatFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stof( literalValue );
}
std::string EcoreFactoryExt::convertEFloatToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<float>(instanceValue);
    return std::to_string( value );
}
Any EcoreFactoryExt::createEIntFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stoi( literalValue );
}
std::string EcoreFactoryExt::convertEIntToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<int>(instanceValue);
    return std::to_string( value );
}
Any EcoreFactoryExt::createELongFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return std::stol( literalValue );
}
std::string EcoreFactoryExt::convertELongToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<long>(instanceValue);
    return std::to_string( value );
}
Any EcoreFactoryExt::createEShortFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    auto i = std::stoi( literalValue );
    if (i >= std::numeric_limits<short>::min() && i <= std::numeric_limits<short>::max())
        return static_cast<short>(i);
    else
        throw std::out_of_range( "" );
}
std::string EcoreFactoryExt::convertEShortToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<short>(instanceValue);
    return std::to_string( value );
}
Any EcoreFactoryExt::createEStringFromString( const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue ) const
{
    return literalValue;
}
std::string EcoreFactoryExt::convertEStringToString( const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue ) const
{
    auto value = anyCast<std::string>(instanceValue);
    return value;
}

EcoreFactoryExt * ecore::ext::EcoreFactoryExt::create()
{
    return new EcoreFactoryExt();
}
