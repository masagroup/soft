// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ECLASSINTERNAL_HPP
#define ECORE_EXT_ECLASSINTERNAL_HPP


namespace ecore::ext
{
    class EClassInternal
    {
    public:
        virtual ~EClassInternal() = default;

        virtual void setModified( int featureID ) = 0;
    };
}

#endif /* ECORE_EXT_ECLASSINTERNAL_HPP */
