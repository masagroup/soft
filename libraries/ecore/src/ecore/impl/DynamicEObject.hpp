// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_DYNAMIC_EOBJECT_HPP_
#define ECORE_DYNAMIC_EOBJECT_HPP_

#include "ecore/impl/BasicEObject.hpp"

namespace ecore::impl
{

    class DynamicEObject : public BasicEObject
    {
    public:
        DynamicEObject();
        DynamicEObject( const std::shared_ptr<EClass>& eClass );
        virtual ~DynamicEObject();
        
        virtual std::shared_ptr<ecore::EClass> eClass() const;

        void setEClass( const std::shared_ptr<EClass>& eClass );

    protected:
        virtual boost::any eGet( int featureID, bool resolve, bool coreType ) const;
        virtual bool eIsSet( int featureID ) const;
        virtual void eSet( int featureID, const boost::any& newValue );
        virtual void eUnset( int featureID );
        virtual boost::any eInvoke( int operationID, const std::shared_ptr<EList<boost::any>>& arguments );

    private:
        int eStaticFeatureCount() const;
        int eStaticOperationCount() const;
        void resizeProperties();

    private:
        class FeaturesAdapter;
        std::unique_ptr<FeaturesAdapter> featuresAdapter_;

    protected:
        std::weak_ptr<EClass> eClass_;
        std::vector< boost::any > properties_;
    };

}

#endif /* ECORE_DYNAMIC_EOBJECT_HPP_ */
