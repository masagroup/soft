// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACT_EOBJECT_ELIST_HPP_
#define ECORE_ABSTRACT_EOBJECT_ELIST_HPP_

#include "ecore/EObject.hpp"
#include "ecore/impl/AbstractENotifyingList.hpp"

namespace ecore::impl
{
    template <typename I, typename S, bool containement, bool inverse, bool opposite>
    class AbstractEObjectEList : public AbstractENotifyingList<I,S>
    {
    public:
        typedef typename AbstractArrayEList<I, S, true> Super;
        typedef typename I InterfaceType;
        typedef typename I::ValueType ValueType;
        typedef typename S StorageType;
        
        template <typename = std::enable_if< std::is_same<ValueType, StorageType>::value>::type>
        AbstractEObjectEList( const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : AbstractENotifyingList<I, S>()
            , owner_( owner )
            , featureID_( featureID )
            , inverseFeatureID_( inverseFeatureID )
            , isSet_( false )
        {
        }

        template <typename = std::enable_if< !std::is_same<ValueType, StorageType>::value>::type>
        AbstractEObjectEList( std::function< ValueType( const StorageType& )> from
                            , std::function< StorageType( const ValueType& )> to
                            , const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : AbstractENotifyingList<I, S>(from , to)
            , owner_( owner )
            , featureID_( featureID )
            , inverseFeatureID_( inverseFeatureID )
            , isSet_( false )
        {
        }

        virtual ~AbstractEObjectEList() 
        {
        }

        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return owner_.lock();
        }

        virtual int getFeatureID() const
        {
            return featureID_;
        }

        virtual std::shared_ptr<EStructuralFeature> getFeature() const
        {
            auto owner = owner_.lock();
            return owner ? owner->eClass()->getEStructuralFeature(featureID_) : nullptr;
        }

        virtual bool isSet() const
        {
            return isSet_;
        }

        virtual void unset()
        {
            clear();
            bool oldIsSet = isSet_;
            isSet_ = false;
            createAndDispatchNotification( nullptr, ENotification::UNSET, oldIsSet, isSet_ , -1);
        }

    protected:

        virtual void didChange()
        {
            isSet_ = true;
        }

        std::shared_ptr< AbstractNotification > createNotification( ENotification::EventType eventType, const Any& oldValue, const Any& newValue, std::size_t position ) const 
        {
            auto owner = owner_.lock();
            return owner ? std::make_shared<Notification>( owner, eventType, featureID_, oldValue, newValue, position ) : nullptr;
        }

        virtual std::shared_ptr<ENotificationChain> inverseAdd( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return doInverseAdd( object, notifications );
        }

        virtual std::shared_ptr<ENotificationChain> inverseRemove( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return doInverseRemove( object, notifications );
        }

        template <bool inv = inverse , bool op = opposite>
        typename std::enable_if< inv && !op, std::shared_ptr<ENotificationChain> >::type doInverseAdd( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return object->getInternal().eInverseAdd( getOwner(), EOPPOSITE_FEATURE_BASE - featureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< inv && op, std::shared_ptr<ENotificationChain> >::type doInverseAdd( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return object->getInternal().eInverseAdd( getOwner(), inverseFeatureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< !inv, std::shared_ptr<ENotificationChain> >::type doInverseAdd( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return notifications;
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< inv && !op, std::shared_ptr<ENotificationChain> >::type doInverseRemove( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return object->getInternal().eInverseRemove( getOwner(), EOPPOSITE_FEATURE_BASE - featureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< inv && op, std::shared_ptr<ENotificationChain> >::type doInverseRemove( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return object->getInternal().eInverseRemove( getOwner(), inverseFeatureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< !inv, std::shared_ptr<ENotificationChain> >::type doInverseRemove( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const
        {
            return notifications;
        }

    private:

        std::shared_ptr<EObject> getOwner() const
        {
            return owner_.lock();
        }
       
    protected:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        int inverseFeatureID_;
        bool isSet_;
    };
}



#endif /* __ECORE__ABSTRACT_EOBJECT_ELIST__HPP__ */
