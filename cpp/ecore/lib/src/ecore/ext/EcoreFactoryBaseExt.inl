// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ECOREFACTORY_ECOREFACTORYBASEEXT_HPP
#error This file may only be included from EFactoryBaseExt.hpp
#endif

#include <ctime>
#include <sstream>

namespace ecore::ext {

    template <typename... I>
    EcoreFactoryBaseExt<I...>::EcoreFactoryBaseExt()
    {
    }

    template <typename... I>
    EcoreFactoryBaseExt<I...>::~EcoreFactoryBaseExt()
    {
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createEBooleanFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        return literalValue == "true" ? true : false;
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertEBooleanToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<bool>(instanceValue);
        return value ? "true" : "false";
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createECharFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        return literalValue[0];
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertECharToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<char>(instanceValue);
        return std::string(1, value);
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createEDateFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        std::istringstream stream(literalValue);
        std::time_t t;
        stream >> t;
        return t;
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertEDateToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<std::time_t>(instanceValue);
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createEDoubleFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        return std::stod(literalValue);
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertEDoubleToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<double>(instanceValue);
        return std::to_string(value);
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createEFloatFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        return std::stof(literalValue);
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertEFloatToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<float>(instanceValue);
        return std::to_string(value);
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createEIntFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        return std::stoi(literalValue);
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertEIntToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<int>(instanceValue);
        return std::to_string(value);
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createELongFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        return std::stol(literalValue);
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertELongToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<long>(instanceValue);
        return std::to_string(value);
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createEShortFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        auto i = std::stoi(literalValue);
        if (i >= std::numeric_limits<short>::min() && i <= std::numeric_limits<short>::max())
            return static_cast<short>(i);
        else
            throw std::out_of_range("");
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertEShortToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<short>(instanceValue);
        return std::to_string(value);
    }

    template <typename... I>
    Any EcoreFactoryBaseExt<I...>::createEStringFromString(const std::shared_ptr<EDataType>& eDataType, const std::string& literalValue) const
    {
        return literalValue;
    }

    template <typename... I>
    std::string EcoreFactoryBaseExt<I...>::convertEStringToString(const std::shared_ptr<EDataType>& eDataType, const Any& instanceValue) const
    {
        auto value = anyCast<std::string>(instanceValue);
        return value;
    }

    template <typename... I>
    EcoreFactoryBaseExt<I...>* EcoreFactoryBaseExt<I...>::create()
    {
        return new EcoreFactoryBaseExt<I...>();
    }

}