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

#include "ecore/BasicElist.hpp"
#include "ecore/EClass.hpp"
#include "ecore/EReference.hpp"
#include "ecore/Notification.hpp"
#include "ecore/NotificationChain.hpp"

#include <memory>
#include <algorithm>

namespace ecore
{
    class BasicEObject;

    template <typename T, bool containement = false, bool inverse = false, bool opposite = false >
    class EObjectEList : public BasicEList<T, true>
    {
        typedef BasicEList<T, true> Super;
    public:
        EObjectEList( const std::shared_ptr<BasicEObject>& owner, int featureID )
            : owner_( owner )
            , featureID_( featureID )
            , inverseFeatureID_( -1 )
            , inverse_( *this )
        {
            std::shared_ptr<EReference> reference = std::dynamic_pointer_cast<EReference>(owner->eClass()->getEStructuralFeature( featureID ));
            if (reference)
            {
                std::shared_ptr<EReference> opposite = reference->getEOpposite();
                if (opposite)
                    inverseFeatureID_ = opposite->getFeatureID();
            }
        }

        EObjectEList( const std::shared_ptr<BasicEObject>& owner, int featureID, int inverseFeatureID )
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
            auto object = std::dynamic_pointer_cast<BasicEObject>(e);
            _ASSERTE( object );
            auto notifications = inverse_.inverseAdd( object, nullptr );
            createAndDispatchNotification( notifications, ENotification::ADD, nullptr, e, index );
        }

        virtual void addUnique( std::size_t index, const T& e )
        {
            Super::addUnique( index, e );
            auto object = std::dynamic_pointer_cast<BasicEObject>(e);
            _ASSERTE( object );
            auto notifications = inverse_.inverseAdd( object, nullptr );
            createAndDispatchNotification( notifications, ENotification::ADD, nullptr, e, index );
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
                auto object = std::dynamic_pointer_cast<BasicEObject>(v_[i + index]);
                _ASSERTE( object );
                notifications = inverse_.inverseAdd( object, notifications );
            }
            createAndDispatchNotification( notifications, [&]() { return l->size() == 1 ? createNotification( ENotification::ADD, nullptr, l->get( 0 ), index ) 
                                                                                        : createNotification( ENotification::ADD_MANY, nullptr, l, index ); } );
            return true;
        }


        virtual T remove( std::size_t index )
        {
            auto oldObject = Super::remove( index );
            auto oldBasicObject = std::dynamic_pointer_cast<BasicEObject>(oldObject);
            _ASSERTE( oldBasicObject );
            auto notifications = inverse_.inverseRemove( oldBasicObject, nullptr );
            createAndDispatchNotification( notifications, ENotification::REMOVE, oldObject, nullptr, index );
            return oldObject;
        }

        virtual T setUnique( std::size_t index, const T& newObject )
        {
            T oldObject = Super::setUnique( index, newObject );
            if (newObject != oldObject)
            {
                auto oldBasicObject = std::dynamic_pointer_cast<BasicEObject>(oldObject);
                auto newBasicObject = std::dynamic_pointer_cast<BasicEObject>(newObject);
                _ASSERTE( oldBasicObject );
                _ASSERTE( newBasicObject );
                std::shared_ptr<ENotificationChain> notifications;
                notifications = inverse_.inverseRemove( oldBasicObject, notifications );
                notifications = inverse_.inverseAdd( newBasicObject, notifications );
                createAndDispatchNotification( notifications, ENotification::SET, oldObject, newObject, index );
            }
            return oldObject;
        }

    protected:

        template <typename Q, bool opposite = false >
        struct Opposite
        {
            Opposite( EObjectEList& list ) : list_( list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return q->eInverseAdd( list_.getOwner(), BasicEObject::EOPPOSITE_FEATURE_BASE - list_.featureID_, notifications );
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return q->eInverseRemove( list_.getOwner(), BasicEObject::EOPPOSITE_FEATURE_BASE - list_.featureID_, notifications );
            }

            EObjectEList& list_;
        };

        template <typename Q>
        struct Opposite<Q, true>
        {
            Opposite( EObjectEList& list ) : list_( list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return q->eInverseAdd( list_.getOwner(), list_.inverseFeatureID_, notifications );
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return q->eInverseRemove( list_.getOwner(), list_.inverseFeatureID_, notifications );
            }

            EObjectEList& list_;
        };

        template <typename Q, bool inverse = false, bool opposite = false >
        struct Inverse
        {
            inline Inverse( EObjectEList& list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return  notifications;
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return  notifications;
            }
        };

        template <typename Q, bool opposite>
        struct Inverse<Q, true, opposite>
        {
            inline Inverse( EObjectEList& list ) : opposite_( list )
            {
            }

            inline std::shared_ptr<ENotificationChain> inverseAdd( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return opposite_.inverseAdd( q, notifications );
            }

            inline std::shared_ptr<ENotificationChain> inverseRemove( const Q& q, const std::shared_ptr<ENotificationChain>& notifications )
            {
                return opposite_.inverseRemove( q, notifications );
            }

            Opposite<Q, opposite> opposite_;
        };

    private:

        std::shared_ptr<BasicEObject> getOwner()
        {
            return owner_.lock();
        }

        bool isNotificationRequired() const
        {
            auto owner = owner_.lock();
            return owner ? owner->eNotificationRequired() : false;
        }

        std::shared_ptr< ENotificationChain > createNotificationChain() const {
            return std::make_shared<NotificationChain>();
        }

        std::shared_ptr< ENotification > createNotification( ENotification::EventType eventType, const boost::any& oldValue, const boost::any& newValue, std::size_t position )
        {
            auto owner = owner_.lock();
            return owner ? std::make_shared<Notification>( eventType, owner, owner->eClass()->getEStructuralFeature( featureID_ ), oldValue, newValue, position ) : nullptr;
        }

        void createAndDispatchNotification( const std::shared_ptr<ENotificationChain>& notifications, ENotification::EventType eventType, const T& oldValue, const T& newValue, std::size_t position )
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
        std::weak_ptr<BasicEObject> owner_;
        int featureID_;
        int inverseFeatureID_;
        Inverse<std::shared_ptr<BasicEObject>, inverse, opposite> inverse_;
    };

}



#endif /* ECORE_EOBJECTELIST_HPP_ */
