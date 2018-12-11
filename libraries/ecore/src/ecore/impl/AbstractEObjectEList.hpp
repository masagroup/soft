// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACTEOBJECTELIST_HPP_
#define ECORE_ABSTRACTEOBJECTELIST_HPP_

#include "ecore/Constants.hpp"
#include "ecore/EObject.hpp"
#include "ecore/impl/Notification.hpp"
#include "ecore/impl/NotificationChain.hpp"
#include "ecore/impl/AbstractArrayEList.hpp"

#include <memory>
#include <algorithm>

#ifdef SHOW_DELETION
#include <iostream>
#endif

namespace ecore::impl
{
    template <typename I, typename S, bool containement, bool inverse, bool opposite>
    class AbstractEObjectEList : public AbstractArrayEList<I,S,true>
    {
    public:
        typedef typename AbstractArrayEList<I, S, true> Super;
        typedef typename I InterfaceType;
        typedef typename I::ValueType ValueType;
        typedef typename S StorageType;
        
        template <typename = std::enable_if< std::is_same<ValueType, StorageType>::value>::type>
        AbstractEObjectEList( const std::weak_ptr<EObject>& owner, int featureID, int inverseFeatureID = -1 )
            : AbstractArrayEList<I, S, true>()
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
            : AbstractArrayEList<I, S, true>( from , to )
            , owner_( owner )
            , featureID_( featureID )
            , inverseFeatureID_( inverseFeatureID )
            , isSet_( false )
        {
        }

        virtual ~AbstractEObjectEList() 
        {
#ifdef SHOW_DELETION
            std::cout << "delete EObjectEList [" << this << "] owner[";
            if( auto owner = owner_.lock() )
                std::cout << owner.get();
            else
                std::cout << "unknown";
            std::cout << "] featureID[" << featureID_ << "]" << std::endl;
#endif
        }

        virtual void addUnique( const ValueType& e )
        {
            auto index = size();
            Super::addUnique( e );
            auto notifications = inverseAdd( e, nullptr );
            createAndDispatchNotification( notifications, ENotification::ADD, NO_VALUE, e, index );
        }

        

        virtual void addUnique( std::size_t index, const ValueType& e )
        {
            Super::addUnique( index, e );
            auto notifications = inverseAdd( e, nullptr );
            createAndDispatchNotification( notifications, ENotification::ADD, NO_VALUE, e, index );
        }

        virtual std::shared_ptr<ENotificationChain> add( const ValueType& e, const std::shared_ptr<ENotificationChain>& notifications )
        {
            auto index = size();
            Super::addUnique( e );
            return createAndAddNotification( notifications, ENotification::ADD, NO_VALUE, e, index );
        }

        virtual bool addAllUnique( const EList<ValueType>& l )
        {
            return addAllUnique( size(), l );
        }

        virtual bool addAllUnique( std::size_t index, const EList<ValueType>& l )
        {
            if (l.empty())
                return false;
            bool result = Super::addAllUnique( index, l );
            auto notifications = createNotificationChain();
            for (int i = 0; i < l.size(); ++i)
            {
                auto object = get(i + index);
                notifications = inverseAdd( object, notifications );
            }
            createAndDispatchNotification( notifications, [&]() { return l.size() == 1 ? createNotification( ENotification::ADD, NO_VALUE, l.get( 0 ), index )
                : createNotification( ENotification::ADD_MANY, NO_VALUE, toAny( l ), index ); } );
            return result;
        }

        virtual ValueType remove( std::size_t index )
        {
            auto oldObject = Super::remove( index );
            auto notifications = inverseRemove( oldObject, nullptr );
            createAndDispatchNotification( notifications, ENotification::REMOVE, oldObject, NO_VALUE, index );
            return oldObject;
        }

        virtual std::shared_ptr<ENotificationChain> remove( const ValueType& e, const std::shared_ptr<ENotificationChain>& notifications )
        {
            std::size_t index = indexOf( e );
            if (index != -1)
            {
                auto oldObject = Super::remove( index );
                return createAndAddNotification( notifications, ENotification::REMOVE, oldObject, NO_VALUE, index );
            }
            return notifications;

        }

        virtual ValueType setUnique( std::size_t index, const ValueType& newObject )
        {
            ValueType oldObject = Super::setUnique( index, newObject );
            if (newObject != oldObject)
            {
                std::shared_ptr<ENotificationChain> notifications;
                notifications = inverseRemove( oldObject, notifications );
                notifications = inverseAdd( newObject, notifications );
                createAndDispatchNotification( notifications, ENotification::SET, oldObject, newObject, index );
            }
            return oldObject;
        }

        virtual std::shared_ptr<ENotificationChain> set( std::size_t index, const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            auto oldObject = Super::setUnique( index, object );
            return createAndAddNotification( notifications, ENotification::SET, oldObject, object, index );
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

        template <bool inv = inverse , bool op = opposite>
        typename std::enable_if< inv && !op, std::shared_ptr<ENotificationChain> >::type inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            return object->eInverseAdd( getOwner(), EOPPOSITE_FEATURE_BASE - featureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< inv && op, std::shared_ptr<ENotificationChain> >::type inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            return object->eInverseAdd( getOwner(), inverseFeatureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< !inv, std::shared_ptr<ENotificationChain> >::type inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            return notifications;
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< inv && !op, std::shared_ptr<ENotificationChain> >::type inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            return object->eInverseRemove( getOwner(), EOPPOSITE_FEATURE_BASE - featureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< inv && op, std::shared_ptr<ENotificationChain> >::type inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            return object->eInverseRemove( getOwner(), inverseFeatureID_, notifications );
        }

        template <bool inv = inverse, bool op = opposite>
        typename std::enable_if< !inv, std::shared_ptr<ENotificationChain> >::type inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            return notifications;
        }

    private:

        static Any toAny( const EList<ValueType>& l )
        {
            std::vector<Any> v;
            std::transform( l.begin(), l.end(), v.end(), []( const ValueType& i ) { return i; } );
            return v;
        }

        std::shared_ptr<EObject> getOwner()
        {
            return owner_.lock();
        }

        bool isNotificationRequired() const
        {
            auto owner = owner_.lock();
            return owner ? (owner->eDeliver() && !owner->eAdapters().empty()) : false;
        }

        std::shared_ptr< ENotificationChain > createNotificationChain() const {
            return std::make_shared<NotificationChain>();
        }

        std::shared_ptr< Notification > createNotification( ENotification::EventType eventType, const Any& oldValue, const Any& newValue, std::size_t position )
        {
            auto owner = owner_.lock();
            return owner ? std::make_shared<Notification>( owner, eventType, featureID_, oldValue, newValue, position ) : nullptr;
        }

        std::shared_ptr<ENotificationChain> createAndAddNotification( const std::shared_ptr<ENotificationChain>& ns, ENotification::EventType eventType, const Any& oldValue, const Any& newValue, std::size_t position )
        {
            std::shared_ptr<ENotificationChain> notifications = ns;
            if (isNotificationRequired())
            {
                auto notification = createNotification( eventType, oldValue, newValue, position );
                if (notifications)
                    notifications->add( notification );
                else
                    notifications = notification;
            }
            return notifications;
        }

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications, ENotification::EventType eventType, const Any& oldValue, const Any& newValue, std::size_t position )
        {
            createAndDispatchNotification( notifications, [&]() { return createNotification( eventType, oldValue, newValue, position ); } );
        }

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications, std::function < std::shared_ptr<ENotification>()> notificationFactory ) {
            if (isNotificationRequired())
            {
                auto notification = notificationFactory();
                if (notifications)
                {
                    notifications->add( notification );
                    notifications->dispatch();
                }
                if (auto owner = owner_.lock())
                    owner->eNotify( notification );
            }
            else
            {
                if (notifications)
                    notifications->dispatch();
            }
        }


    protected:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        int inverseFeatureID_;
        bool isSet_;
    };
}



#endif /* __ECORE__ABSTRACT_EOBJECT_ELIST__HPP__ */
