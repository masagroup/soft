// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_EOBJECTELIST_HPP_
#define ECORE_EOBJECTELIST_HPP_

#include "ecore/Constants.hpp"
#include "ecore/EObject.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EReference.hpp"
#include "ecore/impl/BasicElist.hpp"
#include "ecore/impl/NotifyingElist.hpp"
#include "ecore/impl/Notification.hpp"
#include "ecore/impl/NotificationChain.hpp"

#include <memory>
#include <algorithm>

namespace ecore::impl
{
    
    template <typename T, bool containement = false, bool inverse = false, bool opposite = false >
    class EObjectEList : public BasicEList<T, true> , public NotifyingEList<T>
    {
        typedef BasicEList<T, true> Super;
    public:
        EObjectEList( const std::shared_ptr<EObject>& owner, int featureID )
            : owner_( owner )
            , featureID_( featureID )
            , inverseFeatureID_( -1 )
            , inverse_( *this )
        {
            auto reference = std::dynamic_pointer_cast<EReference>(owner->eClass()->getEStructuralFeature( featureID ));
            if (reference)
            {
                std::shared_ptr<EReference> opposite = reference->getEOpposite();
                if (opposite)
                    inverseFeatureID_ = opposite->getFeatureID();
            }
        }

        EObjectEList( const std::shared_ptr<EObject>& owner, int featureID, int inverseFeatureID )
            : owner_( owner )
            , featureID_( featureID )
            , inverseFeatureID_( inverseFeatureID )
            , inverse_( *this )
        {
        }


        virtual ~EObjectEList()
        {
        }

        virtual void addUnique( const T& e )
        {
            auto index = size();
            Super::addUnique( e );
            auto notifications = inverse_.inverseAdd( e, nullptr );
            createAndDispatchNotification( notifications, ENotification::ADD, NO_VALUE, e, index );
        }

        virtual void addUnique( std::size_t index, const T& e )
        {
            Super::addUnique( index, e );
            auto notifications = inverse_.inverseAdd( e, nullptr );
            createAndDispatchNotification( notifications, ENotification::ADD, NO_VALUE, e, index );
        }

        virtual std::shared_ptr<ENotificationChain> add( const T& e, const std::shared_ptr<ENotificationChain>& notifications )
        {
            auto index = size();
            Super::addUnique( e );
            return createAndAddNotification( notifications, ENotification::ADD, NO_VALUE, e, index );
        }

        virtual bool addAllUnique( const std::shared_ptr<EList<T>>& l )
        {
            return addAllUnique( size(), l );
        }

        virtual bool addAllUnique( std::size_t index, const std::shared_ptr<EList<T>>& l )
        {
            if (l->empty())
                return false;
            Super::addAllUnique( index, l );
            auto notifications = createNotificationChain();
            for (int i = 0; i < l->size(); ++i)
            {
                auto object = v_[ i + index ];
                notifications = inverse_.inverseAdd( object, notifications );
            }
            createAndDispatchNotification( notifications, [&]() { return l->size() == 1 ? createNotification( ENotification::ADD, NO_VALUE, l->get( 0 ), index )
                                                                                        : createNotification( ENotification::ADD_MANY, NO_VALUE, l, index ); } );
            return true;
        }

        virtual T remove( std::size_t index )
        {
            auto oldObject = Super::remove( index );
            auto notifications = inverse_.inverseRemove( oldObject, nullptr );
            createAndDispatchNotification( notifications, ENotification::REMOVE, oldObject, NO_VALUE, index );
            return oldObject;
        }

        virtual std::shared_ptr<ENotificationChain> remove( const T& e, const std::shared_ptr<ENotificationChain>& notifications )
        {
            std::size_t index = indexOf( e );
            if (index != -1)
            {
                auto oldObject = Super::remove( index );
                return createAndAddNotification( notifications, ENotification::REMOVE, oldObject, NO_VALUE, index );
            }
            return notifications;
            
        }

        virtual T setUnique( std::size_t index, const T& newObject )
        {
            T oldObject = Super::setUnique( index, newObject );
            if (newObject != oldObject)
            {
                std::shared_ptr<ENotificationChain> notifications;
                notifications = inverse_.inverseRemove( oldObject, notifications );
                notifications = inverse_.inverseAdd( newObject, notifications );
                createAndDispatchNotification( notifications, ENotification::SET, oldObject, newObject, index );
            }
            return oldObject;
        }

        virtual std::shared_ptr<ENotificationChain> set( std::size_t index, const T& object, const std::shared_ptr<ENotificationChain>& notifications )
        {
            auto oldObject = Super::setUnique( index, object );
            return createAndAddNotification( notifications, ENotification::SET, oldObject, object, index );
        }

    protected:

        template <bool opposite = false >
        struct Opposite
        {
            inline Opposite( EObjectEList& list ) : list_( list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return object->eInverseAdd( list_.getOwner(), EOPPOSITE_FEATURE_BASE - list_.featureID_, notifications );
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return object->eInverseRemove( list_.getOwner(), EOPPOSITE_FEATURE_BASE - list_.featureID_, notifications );
            }

            EObjectEList& list_;
        };

        template <>
        struct Opposite<true>
        {
            inline Opposite( EObjectEList& list ) : list_( list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return object->eInverseAdd( list_.getOwner(), list_.inverseFeatureID_, notifications );
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return object->eInverseRemove( list_.getOwner(), list_.inverseFeatureID_, notifications );
            }

            EObjectEList& list_;
        };

        template <bool inverse = false, bool opposite = false >
        struct Inverse
        {
            inline Inverse( EObjectEList& list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return  notifications;
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return  notifications;
            }
        };

        template <bool opposite>
        struct Inverse<true, opposite>
        {
            inline Inverse( EObjectEList& list ) : opposite_( list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return opposite_.inverseAdd( object, notifications );
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const std::shared_ptr<EObject>& object, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return opposite_.inverseRemove( object, notifications );
            }

            Opposite<opposite> opposite_;
        };

    private:

        std::shared_ptr<EObject> getOwner()
        {
            return owner_.lock();
        }

        bool isNotificationRequired() const
        {
            auto owner = owner_.lock();
            return owner ? ( owner->eDeliver() && !owner->eAdapters().empty() ) : false;
        }

        std::shared_ptr< ENotificationChain > createNotificationChain() const {
            return std::make_shared<NotificationChain>();
        }

        std::shared_ptr< Notification > createNotification( ENotification::EventType eventType, const boost::any& oldValue, const boost::any& newValue, std::size_t position )
        {
            auto owner = owner_.lock();
            return owner ? std::make_shared<Notification>( eventType, owner, owner->eClass()->getEStructuralFeature( featureID_ ), oldValue, newValue, position ) : nullptr;
        }

        std::shared_ptr<ENotificationChain> createAndAddNotification( const std::shared_ptr<ENotificationChain>& ns, ENotification::EventType eventType, const boost::any& oldValue, const boost::any& newValue, std::size_t position )
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

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications, ENotification::EventType eventType, const boost::any& oldValue, const boost::any& newValue, std::size_t position )
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


    private:
        std::weak_ptr<EObject> owner_;
        int featureID_;
        int inverseFeatureID_;
        Inverse<inverse, opposite> inverse_;
    };


}



#endif /* ECORE_EOBJECTELIST_HPP_ */
