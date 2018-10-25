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
#include "ecore/ENotification.hpp"
#include "ecore/EReference.hpp"

#include <memory>
#include <algorithm>

namespace ecore
{
	class BasicEObject;

	template <typename T, bool containement = false, bool inverse = false, bool opposite = false >
	class EObjectEList : public BasicEList<T>
	{
	public:
		EObjectEList(const std::shared_ptr<BasicEObject>& owner, int featureID )
			: owner_(owner)
			, featureID_(featureID)
			, inverseFeatureID_( -1 )
		    , inverse_(*this)
		{
			std::shared_ptr<EReference> reference = std::dynamic_pointer_cast<EReference>( owner->eClass()->getEStructuralFeature(featureID) );
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

		virtual void addUnique(const T& e)
		{
			if (isNotificationRequired())
			{
				std::size_t index = size();
				BasicEList<T>::addUnique( e );
				std::shared_ptr< ENotification > notification = createNotification(ENotification::ADD, nullptr, e, index);
				inverse_.inverseAdd(e);
				dispatchNotification(notification);
			}
			else
				inverse_.inverseAdd(e);
		}
		
	
	protected:

		template <typename Q, bool opposite = false >
		struct Opposite
		{
			Opposite(EObjectEList& list) : list_(list) {}

			inline void inverseAdd(const Q& q) { q->eInverseAdd(list_.owner_, BasicEObject::EOPPOSITE_FEATURE_BASE - list_.featureID_); }

			inline void inverseRemove(const Q& q) { q->eInverseRemove(list_.owner_, BasicEObject::EOPPOSITE_FEATURE_BASE - list_.featureID_); }

			EObjectEList& list_;
		};

		template <typename Q>
		struct Opposite<Q,true>
		{
			Opposite(EObjectEList& list) : list_(list) {}

			inline void inverseAdd(const Q& q) { q->eInverseAdd(list_.owner_, list_.inverseFeatureID_); }

			inline void inverseRemove(const Q& q) { q->eInverseRemove(list_.owner_, list_.inverseFeatureID_); }

			EObjectEList& list_;
		};

		template <typename Q, bool inverse = false, bool opposite = false >
		struct Inverse
		{
			inline Inverse(EObjectEList& list) {}

			inline void inverseAdd(const Q& q) {}

			inline void inverseRemove(const Q& q) {}
		};

		template <typename Q, bool opposite>
		struct Inverse<Q,true,opposite>
		{
			inline Inverse(EObjectEList& list) : opposite_(list) {}

			inline void inverseAdd(const Q& q) { opposite_.inverseAdd(q); }

			inline void inverseRemove(const Q& q) { opposite_.inverseRemove(q);  }

			Opposite<Q,opposite> opposite_;
		};

	private :

		bool isNotificationRequired() const {
			auto owner = owner_.lock();
			return owner ? owner->eNotificationRequired() : false;
		}

		std::shared_ptr< ENotification > createNotification(ENotification::EventType eventType, const T& oldValue, const T& newValue, std::size_t position) const
		{
			auto owner = owner_.lock();
			return owner ? std::make_shared<ENotification>( eventType, owner, owner->eClass()->getEStructuralFeature( featureID_ ), oldValue, newValue, position ) : nullptr;
		}

		void dispatchNotification(const std::shared_ptr<ENotification>& notification) const
		{
			if (auto owner = owner_.lock())
				owner->eNotify(notification);
		}

		
	private:
		std::weak_ptr<BasicEObject> owner_;
		int featureID_;
		int inverseFeatureID_;
		Inverse<T, inverse, opposite> inverse_;
	};

}



#endif /* ECORE_EOBJECTELIST_HPP_ */
