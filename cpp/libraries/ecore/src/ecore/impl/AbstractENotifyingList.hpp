// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ABSTRACT_ENOTIFYING_LIST_HPP_
#define ECORE_ABSTRACT_ENOTIFYING_LIST_HPP_

#include "ecore/Constants.hpp"
#include "ecore/ENotifier.hpp"
#include "ecore/impl/AbstractNotification.hpp"
#include "ecore/impl/NotificationChain.hpp"
#include "ecore/impl/AbstractArrayEList.hpp"

#include <memory>
#include <algorithm>

namespace ecore::impl
{
    template <typename I, typename S>
    class AbstractENotifyingList : public AbstractArrayEList<I, S, true>
    {
    public:
        typedef typename AbstractArrayEList<I, S, true> Super;
        typedef typename I InterfaceType;
        typedef typename I::ValueType ValueType;
        typedef typename S StorageType;

        template <typename = std::enable_if< std::is_same<ValueType, StorageType>::value>::type>
        AbstractENotifyingList()
            : AbstractArrayEList<I, S, true>()
        {
        }

        template <typename = std::enable_if< !std::is_same<ValueType, StorageType>::value>::type>
        AbstractENotifyingList( std::function< ValueType( const StorageType& )> from
                              , std::function< StorageType( const ValueType& )> to )
            : AbstractArrayEList<I, S, true>( from, to )
        {
        }

        virtual ~AbstractENotifyingList()
        {
        }


        virtual std::shared_ptr<ENotifier> getNotifier() const
        {
            return nullptr;
        }

        virtual int getFeatureID() const
        {
            return ENotification::NO_FEATURE_ID;
        }

        virtual std::shared_ptr<EStructuralFeature> getFeature() const
        {
            return nullptr;
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
            if( l.empty() )
                return false;
            bool result = Super::addAllUnique( index, l );
            auto notifications = createNotificationChain();
            for( int i = 0; i < l.size(); ++i )
            {
                auto object = get( i + index );
                notifications = inverseAdd( object, notifications );
            }
            createAndDispatchNotification( notifications, [ & ]()
            {
                return l.size() == 1 ? createNotification( ENotification::ADD, NO_VALUE, l.get( 0 ), index )
                    : createNotification( ENotification::ADD_MANY, NO_VALUE, toAny( l ), index );
            } );
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
            if( index != -1 )
            {
                auto oldObject = Super::remove( index );
                return createAndAddNotification( notifications, ENotification::REMOVE, oldObject, NO_VALUE, index );
            }
            return notifications;

        }

        virtual ValueType setUnique( std::size_t index, const ValueType& newObject )
        {
            ValueType oldObject = Super::setUnique( index, newObject );
            if( newObject != oldObject )
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

    protected:

        virtual std::shared_ptr<ENotificationChain> inverseAdd( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const = 0;
        
        virtual std::shared_ptr<ENotificationChain> inverseRemove( const ValueType& object, const std::shared_ptr<ENotificationChain>& notifications ) const = 0;

        virtual std::shared_ptr< AbstractNotification > createNotification( ENotification::EventType eventType, const Any& oldValue, const Any& newValue, std::size_t position ) const
        {
            class Notification : public AbstractNotification
            {
            public:
                Notification( const AbstractENotifyingList& list
                              , ENotification::EventType eventType
                              , const Any& oldValue
                              , const Any& newValue
                              , std::size_t position )
                    : AbstractNotification( eventType, oldValue, newValue, position )
                    , list_( list )
                {

                }

                virtual std::shared_ptr<ENotifier> getNotifier() const
                {
                    return list_.getNotifier();
                }

                virtual std::shared_ptr<EStructuralFeature> getFeature() const
                {
                    return list_.getFeature();
                }

                virtual int getFeatureID() const
                {
                    return list_.getFeatureID();
                }

            private:
                const AbstractENotifyingList& list_;
            };

            return std::make_shared<Notification>( *this, eventType, oldValue, newValue, position );
        }

        virtual std::shared_ptr< ENotificationChain > createNotificationChain() const
        {
            return std::make_shared<NotificationChain>();
        }

        std::shared_ptr<ENotificationChain> createAndAddNotification( const std::shared_ptr<ENotificationChain>& ns, ENotification::EventType eventType, const Any& oldValue, const Any& newValue, std::size_t position ) const
        {
            std::shared_ptr<ENotificationChain> notifications = ns;
            if( isNotificationRequired() )
            {
                auto notification = createNotification( eventType, oldValue, newValue, position );
                if( notifications )
                    notifications->add( notification );
                else
                    notifications = notification;
            }
            return notifications;
        }

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications, ENotification::EventType eventType, const Any& oldValue, const Any& newValue, std::size_t position ) const
        {
            createAndDispatchNotification( notifications, [ & ]()
            {
                return createNotification( eventType, oldValue, newValue, position );
            } );
        }

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications, std::function < std::shared_ptr<ENotification>()> notificationFactory ) const
        {
            if( isNotificationRequired() )
            {
                auto notification = notificationFactory();
                if( notifications )
                {
                    notifications->add( notification );
                    notifications->dispatch();
                }
                if( auto notifier = getNotifier() )
                    notifier->eNotify( notification );
            }
            else
            {
                if( notifications )
                    notifications->dispatch();
            }
        }

        bool isNotificationRequired() const
        {
            auto notifier = getNotifier();
            return notifier ? ( notifier->eDeliver() && !notifier->eAdapters().empty() ) : false;
        }

    private:

        static Any toAny( const EList<ValueType>& l )
        {
            std::vector<Any> v;
            std::transform( l.begin(), l.end(), v.end(), []( const ValueType& i )
            {
                return i;
            } );
            return v;
        }
    };
}



#endif /* ECORE_ABSTRACT_ENOTIFYING_LIST_HPP_ */
