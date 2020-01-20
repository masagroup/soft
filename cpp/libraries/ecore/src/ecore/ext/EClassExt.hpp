// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2020 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EXT_ECLASSEXT_HPP
#define ECORE_EXT_ECLASSEXT_HPP

#include "ecore/impl/EClassImpl.hpp"

#include <unordered_map>

namespace ecore::ext
{
    template <typename... I>
    class EClassBaseExt : public ecore::impl::EClassBase<I...>
    {
    private:
        EClassBaseExt& operator=( EClassBaseExt const& ) = delete;

    protected:
        friend class impl::EcoreFactoryImpl;
        EClassBaseExt();
        void setThisPtr( const std::shared_ptr<EClassBaseExt>& thisPtr );

    public:
        virtual ~EClassBaseExt();

        //*********************************
        // Operations
        //*********************************
        virtual bool isSuperTypeOf( const std::shared_ptr<ecore::EClass>& someClass );
        virtual int getFeatureCount();
        virtual std::shared_ptr<ecore::EStructuralFeature> getEStructuralFeature( int featureID );
        virtual std::shared_ptr<ecore::EStructuralFeature> getEStructuralFeature( const std::string& featureName );
        virtual int getFeatureID( const std::shared_ptr<ecore::EStructuralFeature>& feature );
        virtual int getOperationCount();
        virtual std::shared_ptr<ecore::EOperation> getEOperation( int operationID );
        virtual int getOperationID( const std::shared_ptr<ecore::EOperation>& operation );
        virtual std::shared_ptr<ecore::EOperation> getOverride( const std::shared_ptr<ecore::EOperation>& operation );
       
        void setModified( int featureID );
    
    protected:
        //*********************************
        // Initializers
        //*********************************
        virtual void initEReferences();
        virtual void initEAttributes();
        virtual void initEContainments();
        virtual void initECrossReferences();
        virtual void initEAllAttributes();
        virtual void initEAllReferences();
        virtual void initEAllContainments();
        virtual void initEAllOperations();
        virtual void initEAllStructuralFeatures();
        virtual void initEAllSuperTypes();
        virtual void initEIDAttribute();
   
    private:
        void initFeaturesSubSet();
        void initNameToFeatureMap();
        void initOperationToOverrideMap();

    private:
        class ESuperAdapter;
        std::unique_ptr<ESuperAdapter> eSuperAdapter_;
        std::unique_ptr< std::unordered_map< std::string, std::shared_ptr<EStructuralFeature>>> nameToFeatureMap_;
        std::unique_ptr< std::unordered_map< std::shared_ptr<EOperation>, std::shared_ptr<EOperation>>> operationToOverrideMap_;
    };

    typedef EClassBaseExt<EClass> EClassExt;
}

#include "ecore/ext/EClassExt.inl"

#endif /* ECORE_EXT_ECLASSEXT_HPP */
